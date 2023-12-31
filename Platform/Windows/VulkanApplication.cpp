#include "VulkanApplication.h"
#include "Logging.h"

/**
* Construct a new VulkanApplication.
* 
* @param width	the width of the window
* @param height the height of the window
* @param debug	whether to run the VulkanApplication with vulkan validation layers and extra print statements
*/
VulkanApplication::VulkanApplication(int width, int height, bool debug) {

    vkLogging::Logger::get_logger()->set_debug_mode(debug);

    build_glfw_window(width, height);

    graphicsEngine = new VulkanGraphicsManager(width, height, window);

    scene = new Scene();
}

/**
* Build the VulkanVulkanApplicationlication's window (using glfw)
* 
* @param width		the width of the window
* @param height		the height of the window
* @param debugMode	whether to make extra print statements
*/
void VulkanApplication::build_glfw_window(int width, int height) {

    std::stringstream message;

    //initialize glfw
    glfwInit();

    //no default rendering client, we'll hook vulkan up
    //to the window later
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    //resizing breaks the swapchain, we'll disable it for now
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    //GLFWwindow* glfwCreateWindow (int width, int height, const char *title, GLFWmonitor *monitor, GLFWwindow *share)
    if (window =
            glfwCreateWindow(width, height, "ID Tech 12", nullptr, nullptr)) {
        message
            << "Successfully made a glfw window called \"ID Tech 12\", width: "
            << width << ", height: " << height;
        vkLogging::Logger::get_logger()->print(message.str());
    } else {
        vkLogging::Logger::get_logger()->print("GLFW window creation failed");
    }
}

/**
* Start the VulkanApplication's main loop
*/
void VulkanApplication::run() {

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        graphicsEngine->render(scene);
        calculateFrameRate();
    }
}

/**
* Calculates the VulkanApplication's framerate and updates the window title
*/
void VulkanApplication::calculateFrameRate() {
    currentTime = glfwGetTime();
    double delta = currentTime - lastTime;

    if (delta >= 1) {
        int framerate{std::max(1, int(numFrames / delta))};
        std::stringstream title;
        title << "Running at " << framerate << " fps.";
        glfwSetWindowTitle(window, title.str().c_str());
        lastTime = currentTime;
        numFrames = -1;
        frameTime = float(1000.0 / framerate);
    }

    ++numFrames;
}

/**
* VulkanApplication destructor.
*/
VulkanApplication::~VulkanApplication() {
    delete graphicsEngine;
    delete scene;
}