#pragma once
#include <vulkan/vulkan.hpp>

#include <GLFW/glfw3.h>

#include <fstream>
#include <iostream>
#include <mutex>
#include <optional>
#include <set>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

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
enum class meshTypes { GIRL, GROUND, SKULL };

enum class pipelineType { SKY, STANDARD };

// String Processing functions

std::vector<std::string> split(std::string line, std::string delimiter);