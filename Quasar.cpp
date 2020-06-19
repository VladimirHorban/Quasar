#include "Quasar.h"

using namespace std;

LRESULT CALLBACK WindowProcessMessages(HWND hwnd, UINT msg, WPARAM param, LPARAM lparam);

int WINAPI main(HINSTANCE currentInstance, HINSTANCE previousInstance, PSTR cmdLine, INT cmdCount)
{
    // Register the window class
    const char* CLASS_NAME = "Window";
    WNDCLASS wc{};
    wc.hInstance = currentInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpfnWndProc = WindowProcessMessages;
    RegisterClass(&wc);

    // Create the window
    HWND pHwnd = CreateWindow(CLASS_NAME, "Vulkan window",
                              WS_OVERLAPPEDWINDOW | WS_VISIBLE,			// Window style
                              CW_USEDEFAULT, CW_USEDEFAULT,				// Window initial position
                              800, 600,			            			// Window size
                              nullptr, nullptr, nullptr, nullptr);

    // Initialize core    
    Instance        instance("QuasarSample", "Quasar", 1, 0, 0);
    Validation      validation(instance.getVkInstance());
    WindowSurface   winSurface(instance.getVkInstance(), pHwnd, currentInstance);
    PhysicalDevice  physicalDevice(instance.getVkInstance(), winSurface.getVkSurfaceKHR());
    LogicalDevice   device(physicalDevice, winSurface);
    SwapChain       swapChain(*device.getDevice(), physicalDevice.getVkPhysicalDevice(), winSurface.getVkSurfaceKHR(), physicalDevice.getQueueFamilyIndices());
    GraphicPipeline graphicPipeline(device.getDevice(), &swapChain.getSwapChainExtend(), swapChain.getSwapChainFormat());
    Framebuffer     framebuffer(*device.getDevice(), *graphicPipeline.getVkRenderPass(), swapChain.getSwapChainExtend(), swapChain.getSwapChainImageViews(),swapChain.getSwapChainImageViewsSize());
    Commandbuffer   commandbuffer(device.getDevice(), graphicPipeline.getGraphicPipeline(), graphicPipeline.getVkRenderPass(), &framebuffer, &swapChain.getSwapChainExtend(), physicalDevice.getQueueFamilyIndices());

    // Test
    TestApp app(&device, &swapChain, &commandbuffer, &graphicPipeline);
    app.run();

    // Window loop
    MSG msg{};
    while (GetMessage(&msg, nullptr, 0, 0)) 
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    cout << "Hello I am Quasar!" << endl;

    return 0;
}

LRESULT CALLBACK WindowProcessMessages(HWND hwnd, UINT msg, WPARAM param, LPARAM lparam) {
    switch (msg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hwnd, msg, param, lparam);
    }
}

void TestApp::run()
{
    mainLoop();
}

void TestApp::mainLoop()
{
    createSyncObjects();
    while (true)
    {
        drawFrame();
    }
}

void TestApp::drawFrame()
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

void TestApp::createSyncObjects()
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

