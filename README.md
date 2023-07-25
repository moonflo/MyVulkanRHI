# MyVulkanRHI
## brief
My vulkan RHI design playground, explore how VulkanRHI should be designed and used to integrate it into my [GameEngineFromStratch](https://github.com/moonflo/EngineFromScratch) repo. Notice that currently this code only tested on windows.
## Install
1. Download [VulkanSDK](https://www.lunarg.com/vulkan-sdk/) on your pc. My approach is to download the corresponding version of the installer and install it in a location of random choice. There are already copies of all the vulkan header files to be used in external/windows/include, and cmake will check and try to find their lib files. Normally, after installing VulkanSDK, there is no need to do anything else, but just in case, if you encounter strange problems related to the compilation environment, one should make a new copy of the header files in the local installation directory.;
1. Clone this repo (Make sure it's a clone but NOT download as ZIP file, for we have a submodule in this);
2. Run build_all.bat for the initial install of all external dependencies;
3. visual studio sln is in ./build, for any code modification one should only run the rebuild.bat for it;

## TODO:
1. Check my code in others pc, make sure this code is as flexible as possible;


# MyVulkanRHI
## 简介
我的vulkanRHI设计游乐场，用于探索如何设计VulkanRHI并将其集成到我的 [GameEngineFromStratch](https://github.com/moonflo/EngineFromScratch) 存储库中。请注意，目前此代码仅在Windows上进行了测试。
## 安装
1. 在您的电脑上下载[VulkanSDK](https://www.lunarg.com/vulkan-sdk/)。 我的做法是下载对应版本的安装程序并安装在随机选择的位置。external/windows/include中已经有要使用的所有vulkan头文件的副本，而在编译时，cmake将检查并尝试找到它们的lib文件。 通常情况下，安装VulkanSDK后，无需执行任何其他操作，cmake能找到lib文件，但以防万一，如果遇到与编译环境相关的奇怪问题，应从本地安装目录中重新复制一次头文件；
2.克隆这个repo（必须是克隆不能是下载为ZIP，因为其中有一个子模块，子模块不会被下载）；
3. 运行build_all.bat来初始安装所有外部依赖项并且在build中构建vs解决方案，如果vs版本和我的不一致，需要修改一下批处理文件中的设置；
4. Visual Studio sln位于./build中，在build_all之后，再修改代码只需要运行rebuild.bat就行了；

## 待办：
1.在其他人的pc上检查代码是否能够运行；
