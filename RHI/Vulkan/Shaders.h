#pragma once
#include "VulkanConfig.h"

namespace vkUtil {

/**
		Read a file.

		\param filename a string representing the path to the file
		\param debug whether the system is running in debug mode
		\returns the contents as a vector of raw binary characters
	*/
std::vector<char> readFile(std::string filename, bool debug) {

    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (debug && !file.is_open()) {
        std::cout << "[ERROR] Failed to load \"" << filename << "\""
                  << std::endl;
    }

    size_t filesize{static_cast<size_t>(file.tellg())};

    std::vector<char> buffer(filesize);
    file.seekg(0);
    file.read(buffer.data(), filesize);

    file.close();
    return buffer;
}

/**
		Search file relative address.

		\param name a string representing the path to the file
		\param debug whether the system is running in debug mode
		\returns the relative path form exe file, empty string if not found.
	*/
std::string relativePathSearching(std::string name, bool debug) {
    // loop N times up the hierarchy, testing at each level
    std::string upPath;
    std::string fullPath;
    for (int32_t i = 0; i < 10; i++) {

        fullPath.assign(upPath);
        fullPath.append(name);
        if (debug) {
            std::cout << "[INFO] Searching file in path: " << fullPath.c_str()
                      << std::endl;
        }
        // check in file is in here
        std::ifstream file(fullPath);
        if (file.good()) {
            file.close();
            if (debug) {
                std::cout << "[INFO] Found file in path: " << fullPath.c_str()
                          << std::endl;
            }
            return fullPath;
            break;
        }
        upPath.append("../");
    }
    return std::string();
}

/**
		Make a shader module.

		\param filename a string holding the filepath to the spir-v file.
		\param device the logical device
		\param debug whether the system is running in debug mode
		\returns the created shader module
	*/
vk::ShaderModule createModule(std::string filename, vk::Device device,
                              bool debug) {
    std::string newPath = relativePathSearching(filename, debug);
    if (newPath == "") {
        if (debug) {
            std::cout << "[ERROR] Failed to locating file: " << newPath
                      << std::endl;
        }
    } else {
        filename = newPath;
    }
    std::vector<char> sourceCode = readFile(filename, debug);
    vk::ShaderModuleCreateInfo moduleInfo = {};
    moduleInfo.flags = vk::ShaderModuleCreateFlags();
    moduleInfo.codeSize = sourceCode.size();
    moduleInfo.pCode = reinterpret_cast<const uint32_t*>(sourceCode.data());
    try {
        return device.createShaderModule(moduleInfo);
    } catch (vk::SystemError err) {
        if (debug) {
            std::cout << "Failed to create shader module for \"" << filename
                      << "\"" << std::endl;
        }
    }
}
}  // namespace vkUtil