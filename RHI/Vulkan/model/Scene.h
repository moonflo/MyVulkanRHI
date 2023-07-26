#pragma once
#include "VulkanConfig.h"

class Scene {

   public:
    Scene();

    std::vector<glm::vec3> trianglePositions;

    std::vector<glm::vec3> squarePositions;

    std::vector<glm::vec3> starPositions;
};