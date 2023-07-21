#pragma once
#include "VulkanConfig.h"
#include <GLFW/glfw3.h>

class Engine
{

public:
	Engine();

	~Engine();

private:
	// whether to print debug messages in functions
	bool debugMode = true;

	// glfw window parameters
	int width{640};
	int height{480};
	GLFWwindow *window{nullptr};

	// vulkan instance
	vk::Instance instance{nullptr};

	// glfw setup
	/**
		Build the GLFW window.
	*/
	void build_glfw_window();

	// instance setup
	/**
		Create the Vulkan instance.
	*/
	void make_instance();
};