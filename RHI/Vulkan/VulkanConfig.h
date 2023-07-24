#pragma once
#include <vulkan/vulkan.hpp>
/*
* Including the prebuilt header from the lunarg sdk will load
* most functions, but not all.
*
* Functions can also be dynamically loaded, using the call
*
* PFN_vkVoidFunction vkGetInstanceProcAddr(
	VkInstance                                  instance,
	const char*                                 pName);

 or

 PFN_vkVoidFunction vkGetDeviceProcAddr(
	VkDevice                                    device,
	const char*                                 pName);

	We will look at this later, once we've created an instance and device.
*/

#include <GLFW/glfw3.h>

#include <fstream>
#include <iostream>
#include <optional>
#include <set>
#include <string>
#include <vector>
