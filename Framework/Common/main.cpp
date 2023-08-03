#include "VulkanApplication.h"

int main() {

    VulkanApplication* myApp = new VulkanApplication(640, 480, true);

    myApp->run();
    delete myApp;

    return 0;
}