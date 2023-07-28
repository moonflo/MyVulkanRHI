#pragma once
#include "Scene.h"
#include "VulkanConfig.h"
#include "VulkanGraphicsManager.h"

class App {

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
    App(int width, int height, bool debug);
    ~App();
    void run();
};