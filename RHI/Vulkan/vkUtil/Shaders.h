#pragma once
#include "VulkanConfig.h"

namespace vkUtil {

/**
		Read a file.

		\param filename a string representing the path to the file
		\returns the contents as a vector of raw binary characters
	*/
std::vector<char> readFile(std::string filename);

/**
		Search file relative address.

		\param name a string representing the path to the file
		\param debug whether the system is running in debug mode
		\returns the relative path form exe file, empty string if not found.
	*/
std::string relativePathSearching(std::string name);

/**
		Make a shader module.

		\param filename a string holding the filepath to the spir-v file.
		\param device the logical device
		\param debug whether the system is running in debug mode
		\returns the created shader module
	*/
vk::ShaderModule createModule(std::string filename, vk::Device device);
}  // namespace vkUtil