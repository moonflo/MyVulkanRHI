add_executable(VulkanApplication
VulkanApplication.cpp)

find_package(Vulkan REQUIRED)
target_link_libraries(VulkanApplication Common MyVulkanRHI Vulkan::Vulkan ${GLFW_LIBRARY})