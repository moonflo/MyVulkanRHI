#pragma once
#include <vulkan/vulkan.hpp>

#include <GLFW/glfw3.h>

#include <fstream>
#include <iostream>
#include <optional>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Logging.h"
/**
	Data structures used for creating buffers
	and allocating memory
*/
struct BufferInputChunk {
    size_t size;
    vk::BufferUsageFlags usage;
    vk::Device logicalDevice;
    vk::PhysicalDevice physicalDevice;
    vk::MemoryPropertyFlags memoryProperties;
};

/**
	holds a vulkan buffer and memory allocation
*/
struct Buffer {
    vk::Buffer buffer;
    vk::DeviceMemory bufferMemory;
};

//--------- Assets -------------//
enum class meshTypes { TRIANGLE, SQUARE, STAR };