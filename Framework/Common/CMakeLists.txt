add_library(Common
Logging.cpp
Scene.cpp
main.cpp
)
find_package(Vulkan REQUIRED)
target_link_libraries(Common Vulkan::Vulkan ${GLFW_LIBRARY})