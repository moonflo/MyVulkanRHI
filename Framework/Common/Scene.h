#pragma once
#include "VulkanConfig.h"

class Scene {

   public:
    Scene();

    std::unordered_map<meshTypes, std::vector<glm::vec3>> positions;
};