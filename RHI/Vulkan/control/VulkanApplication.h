#pragma once
#include "Engine.h"
#include "Scene.h"
#include "VulkanConfig.h"

class App {

   private:
    Engine* graphicsEngine;
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