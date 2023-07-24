#pragma once
#include "VulkanConfig.h"

namespace vkUtil {

/**
		Holds the data structures associated with a "Frame"
	*/
struct SwapChainFrame {
    vk::Image image;
    vk::ImageView imageView;
};

}  // namespace vkUtil