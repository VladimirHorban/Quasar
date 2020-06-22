#include "../Headers/Core.h"

VulkanCore::VulkanCore(const char* aAppName, int aWidth, int aHeight, HINSTANCE aCurrentInstance, const char* mVertexShader, const char* mFragmentShader)
{
    init(aAppName, aWidth, aHeight, aCurrentInstance,mVertexShader, mFragmentShader);
}

VulkanCore::~VulkanCore()
{
    delete mWindow;
    delete mDevice;
    delete mSwapChain;
    delete mCommandbuffer;
    delete mFramebuffer;
    delete mGraphicPipeline;
}

void VulkanCore::init(const char* aAppName, int aWidth, int aHeight, HINSTANCE aCurrentInstance, const char* mVertexShader, const char* mFragmentShader)
{
    Instance        instance(aAppName, "Quasar", 1, 0, 0);
    Validation      validation(instance.getVkInstance());    
    mWindow         = new Window(aCurrentInstance, aAppName, aWidth, aHeight);
    WindowSurface   winSurface(instance.getVkInstance(), mWindow->getPHWND(), aCurrentInstance);
    PhysicalDevice  physicalDevice(instance.getVkInstance(), winSurface.getVkSurfaceKHR());

    
    mDevice          = new LogicalDevice(physicalDevice, winSurface);    
    mSwapChain       = new SwapChain(*mDevice->getDevice(), physicalDevice.getVkPhysicalDevice(), winSurface.getVkSurfaceKHR(), physicalDevice.getQueueFamilyIndices());   
    mGraphicPipeline = new GraphicPipeline(mDevice->getDevice(), &mSwapChain->getSwapChainExtend(), mSwapChain->getSwapChainFormat(), mVertexShader, mFragmentShader);    
    mFramebuffer     = new Framebuffer(*mDevice->getDevice(), *mGraphicPipeline->getVkRenderPass(), mSwapChain->getSwapChainExtend(), mSwapChain->getSwapChainImageViews(), mSwapChain->getSwapChainImageViewsSize());    
    mCommandbuffer   = new Commandbuffer(mDevice->getDevice(), mGraphicPipeline->getGraphicPipeline(), mGraphicPipeline->getVkRenderPass(), mFramebuffer, &mSwapChain->getSwapChainExtend(), physicalDevice.getQueueFamilyIndices());
}

void VulkanCore::run()
{
    mainLoop();
}

void VulkanCore::mainLoop()
{
    createSyncObjects();
    while (true)
    {
        drawFrame();
    }
}

void VulkanCore::drawFrame()
{
    vkWaitForFences(*mDevice->getDevice(), 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    vkAcquireNextImageKHR(*mDevice->getDevice(), mSwapChain->getSwapChain(), UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

    if (imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
        vkWaitForFences(*mDevice->getDevice(), 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
    }
    imagesInFlight[imageIndex] = inFlightFences[currentFrame];

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    //submitInfo.pCommandBuffers = &commandBuffers[imageIndex];
    auto command = mCommandbuffer->getCommandBuffers()[imageIndex];
    //submitInfo.pCommandBuffers = &mCommandbuffer->getCommandBuffers()[imageIndex];
    submitInfo.pCommandBuffers = &command;

    VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    vkResetFences(*mDevice->getDevice(), 1, &inFlightFences[currentFrame]);

    if (vkQueueSubmit(*mDevice->getGraphicsQueue(), 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = { mSwapChain->getSwapChain() };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &imageIndex;

    vkQueuePresentKHR(*mDevice->getPresentQueue(), &presentInfo);

    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

}

void VulkanCore::createSyncObjects()
{
    imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
    imagesInFlight.resize(mSwapChain->getSwapChainImageViewsSize(), VK_NULL_HANDLE);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        if (vkCreateSemaphore(*mDevice->getDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(*mDevice->getDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(*mDevice->getDevice(), &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create synchronization objects for a frame!");
        }
    }
}

void VulkanCore::addShader(const char* mVertexShader, const char* mFragmentShader)
{

}