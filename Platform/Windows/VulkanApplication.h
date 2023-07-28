#pragma once
#include "Scene.h"
#include "VulkanConfig.h"
#include "VulkanGraphicsManager.h"

class VulkanApplication {

   private:
    VulkanGraphicsManager* graphicsEngine;
    GLFWwindow* window;
    Scene* scene;

    double lastTime, currentTime;
    int numFrames;
    float frameTime;

    void build_glfw_window(int width, int height);

    void calculateFrameRate();

   public:
    VulkanApplication(int width, int height, bool debug);
    ~VulkanApplication();
    void run();
};