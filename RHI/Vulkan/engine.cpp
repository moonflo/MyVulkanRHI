#include "Engine.h"
#include "Commands.h"
#include "Device.h"
#include "Framebuffer.h"
#include "Instance.h"
#include "Logging.h"
#include "Pipeline.h"
#include "Swapchain.h"
#include "Sync.h"

Engine::Engine(int width, int height, GLFWwindow* window, bool debug) {

    this->width = width;
    this->height = height;
    this->window = window;
    debugMode = debug;

    if (debugMode) {
        std::cout << "Making a graphics engine\n";
    }

    make_instance();

    make_device();

    make_pipeline();

    finalize_setup();
}

void Engine::make_instance() {

    instance = vkInit::make_instance(debugMode, "ID Tech 12");
    dldi = vk::DispatchLoaderDynamic(instance, vkGetInstanceProcAddr);
    if (debugMode) {
        debugMessenger = vkInit::make_debug_messenger(instance, dldi);
    }
    VkSurfaceKHR c_style_surface;
    if (glfwCreateWindowSurface(instance, window, nullptr, &c_style_surface) !=
        VK_SUCCESS) {
        if (debugMode) {
            std::cout << "Failed to abstract glfw surface for Vulkan\n";
        }
    } else if (debugMode) {
        std::cout << "Successfully abstracted glfw surface for Vulkan\n";
    }
    //copy constructor converts to hpp convention
    surface = c_style_surface;
}

void Engine::make_device() {

    physicalDevice = vkInit::choose_physical_device(instance, debugMode);
    device = vkInit::create_logical_device(physicalDevice, surface, debugMode);
    std::array<vk::Queue, 2> queues =
        vkInit::get_queues(physicalDevice, device, surface, debugMode);
    graphicsQueue = queues[0];
    presentQueue = queues[1];
    make_swapchain();
    frameNumber = 0;
}

/**
* Make a swapchain
*/
void Engine::make_swapchain() {

    vkInit::SwapChainBundle bundle = vkInit::create_swapchain(
        device, physicalDevice, surface, width, height, debugMode);
    swapchain = bundle.swapchain;
    swapchainFrames = bundle.frames;
    swapchainFormat = bundle.format;
    swapchainExtent = bundle.extent;
    maxFramesInFlight = static_cast<int>(swapchainFrames.size());
}

/**
* The swapchain must be recreated upon resize or minimization, among other cases
*/
void Engine::recreate_swapchain() {

    width = 0;
    height = 0;
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(window, &width, &height);
        glfwWaitEvents();
    }

    device.waitIdle();

    cleanup_swapchain();
    make_swapchain();
    make_framebuffers();
    make_frame_sync_objects();
    vkInit::commandBufferInputChunk commandBufferInput = {device, commandPool,
                                                          swapchainFrames};
    vkInit::make_frame_command_buffers(commandBufferInput, debugMode);
}

void Engine::make_pipeline() {

    vkInit::GraphicsPipelineInBundle specification = {};
    specification.device = device;
    specification.vertexFilepath = "Asset/Shaders/vertex.spv";
    specification.fragmentFilepath = "Asset/Shaders/fragment.spv";
    specification.swapchainExtent = swapchainExtent;
    specification.swapchainImageFormat = swapchainFormat;

    vkInit::GraphicsPipelineOutBundle output = vkInit::create_graphics_pipeline(
        mainCommandBuffer, specification, debugMode);

    pipelineLayout = output.layout;
    renderpass = output.renderpass;
    pipeline = output.pipeline;
}

/**
* Make a framebuffer for each frame
*/
void Engine::make_framebuffers() {

    vkInit::framebufferInput frameBufferInput;
    frameBufferInput.device = device;
    frameBufferInput.renderpass = renderpass;
    frameBufferInput.swapchainExtent = swapchainExtent;
    vkInit::make_framebuffers(frameBufferInput, swapchainFrames, debugMode);
}

/**
* Make synchronization objects for each frame
*/
void Engine::make_frame_sync_objects() {

    for (vkUtil::SwapChainFrame& frame : swapchainFrames) {
        frame.imageAvailable = vkInit::make_semaphore(device, debugMode);
        frame.renderFinished = vkInit::make_semaphore(device, debugMode);
        frame.inFlight = vkInit::make_fence(device, debugMode);
    }
}

void Engine::finalize_setup() {

    make_framebuffers();

    commandPool =
        vkInit::make_command_pool(device, physicalDevice, surface, debugMode);

    vkInit::commandBufferInputChunk commandBufferInput = {device, commandPool,
                                                          swapchainFrames};
    mainCommandBuffer =
        vkInit::make_command_buffer(commandBufferInput, debugMode);
    vkInit::make_frame_command_buffers(commandBufferInput, debugMode);

    make_frame_sync_objects();
}

void Engine::record_draw_commands(vk::CommandBuffer commandBuffer,
                                  uint32_t imageIndex, Scene* scene) {

    vk::CommandBufferBeginInfo beginInfo = {};

    try {
        commandBuffer.begin(beginInfo);
    } catch (vk::SystemError err) {
        if (debugMode) {
            std::cout << "Failed to begin recording command buffer!"
                      << std::endl;
        }
    }

    vk::RenderPassBeginInfo renderPassInfo = {};
    renderPassInfo.renderPass = renderpass;
    renderPassInfo.framebuffer = swapchainFrames[imageIndex].framebuffer;
    renderPassInfo.renderArea.offset.x = 0;
    renderPassInfo.renderArea.offset.y = 0;
    renderPassInfo.renderArea.extent = swapchainExtent;

    vk::ClearValue clearColor = {std::array<float, 4>{1.0f, 0.5f, 0.25f, 1.0f}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    commandBuffer.beginRenderPass(&renderPassInfo,
                                  vk::SubpassContents::eInline);

    commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline);

    // start draw call
    //  reset viewport and scissor
    vk::Viewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)swapchainExtent.width;
    viewport.height = (float)swapchainExtent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vk::Rect2D scissor{};
    scissor.offset.x = 0;
    scissor.offset.y = 0;
    scissor.extent = swapchainExtent;
    // setting viewport and scissor
    commandBuffer.setViewport(0, 1, &viewport);
    commandBuffer.setScissor(0, 1, &scissor);

    // render
    for (glm::vec3 position : scene->trianglePositions) {

        glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
        vkUtil::ObjectData objectData;
        objectData.model = model;
        commandBuffer.pushConstants(pipelineLayout,
                                    vk::ShaderStageFlagBits::eVertex, 0,
                                    sizeof(objectData), &objectData);

        commandBuffer.draw(3, 1, 0, 0);
    }

    commandBuffer.endRenderPass();

    try {
        commandBuffer.end();
    } catch (vk::SystemError err) {

        if (debugMode) {
            std::cout << "failed to record command buffer!" << std::endl;
        }
    }
}

void Engine::render(Scene* scene) {

    device.waitForFences(1, &(swapchainFrames[frameNumber].inFlight), VK_TRUE,
                         UINT64_MAX);
    device.resetFences(1, &(swapchainFrames[frameNumber].inFlight));

    //acquireNextImageKHR(vk::SwapChainKHR, timeout, semaphore_to_signal, fence)
    uint32_t imageIndex;
    try {
        vk::ResultValue acquire = device.acquireNextImageKHR(
            swapchain, UINT64_MAX, swapchainFrames[frameNumber].imageAvailable,
            nullptr);
        imageIndex = acquire.value;
    } catch (vk::OutOfDateKHRError error) {
        std::cout << "Recreate" << std::endl;
        recreate_swapchain();
        return;
    } catch (vk::IncompatibleDisplayKHRError error) {
        std::cout << "Recreate" << std::endl;
        recreate_swapchain();
        return;
    } catch (vk::SystemError error) {
        std::cout << "Failed to acquire swapchain image!" << std::endl;
    }

    vk::CommandBuffer commandBuffer =
        swapchainFrames[frameNumber].commandBuffer;

    commandBuffer.reset();

    record_draw_commands(commandBuffer, imageIndex, scene);

    vk::SubmitInfo submitInfo = {};

    vk::Semaphore waitSemaphores[] = {
        swapchainFrames[frameNumber].imageAvailable};
    vk::PipelineStageFlags waitStages[] = {
        vk::PipelineStageFlagBits::eColorAttachmentOutput};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vk::Semaphore signalSemaphores[] = {
        swapchainFrames[frameNumber].renderFinished};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    try {
        graphicsQueue.submit(submitInfo, swapchainFrames[frameNumber].inFlight);
    } catch (vk::SystemError err) {

        if (debugMode) {
            std::cout << "failed to submit draw command buffer!" << std::endl;
        }
    }

    vk::PresentInfoKHR presentInfo = {};
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    vk::SwapchainKHR swapChains[] = {swapchain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &imageIndex;

    vk::Result present;

    try {
        present = presentQueue.presentKHR(presentInfo);
    } catch (vk::OutOfDateKHRError error) {
        present = vk::Result::eErrorOutOfDateKHR;
    }

    if (present == vk::Result::eErrorOutOfDateKHR ||
        present == vk::Result::eSuboptimalKHR) {
        std::cout << "Recreate" << std::endl;
        recreate_swapchain();
        return;
    }

    frameNumber = (frameNumber + 1) % maxFramesInFlight;
}

/**
* Free the memory associated with the swapchain objects
*/
void Engine::cleanup_swapchain() {

    for (vkUtil::SwapChainFrame frame : swapchainFrames) {
        device.destroyImageView(frame.imageView);
        device.destroyFramebuffer(frame.framebuffer);
        device.destroyFence(frame.inFlight);
        device.destroySemaphore(frame.imageAvailable);
        device.destroySemaphore(frame.renderFinished);
    }
    device.destroySwapchainKHR(swapchain);
}

Engine::~Engine() {

    device.waitIdle();

    if (debugMode) {
        std::cout << "Goodbye see you!\n";
    }

    device.destroyCommandPool(commandPool);

    device.destroyPipeline(pipeline);
    device.destroyPipelineLayout(pipelineLayout);
    device.destroyRenderPass(renderpass);

    cleanup_swapchain();

    device.destroy();

    instance.destroySurfaceKHR(surface);
    if (debugMode) {
        instance.destroyDebugUtilsMessengerEXT(debugMessenger, nullptr, dldi);
    }
    /*
	* from vulkan_funcs.hpp:
	* 
	* void Instance::destroy( Optional<const VULKAN_HPP_NAMESPACE::AllocationCallbacks> allocator = nullptr,
                                            Dispatch const & d = ::vk::getDispatchLoaderStatic())
	*/
    instance.destroy();

    //terminate glfw
    glfwTerminate();
}