#ifndef CORE_H
#define CORE_H

#include <iostream>
#include "Window.h"
#include "Validation.h"
#include "Instance.h"
#include "PhysicalDevice.h"
#include "WindowSurface.h"
#include "LogicalDevice.h"
#include "Window.h"
#include "SwapChain.h"
#include "Pipeline.h"
#include "FrameBuffer.h"
#include "CommandBuffer.h"

class VulkanCore
{

private:
	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence>     inFlightFences;
	std::vector<VkFence>     imagesInFlight;
	size_t currentFrame = 0;

	const int MAX_FRAMES_IN_FLIGHT = 2;

	Window*          mWindow          = nullptr;
	LogicalDevice*   mDevice          = nullptr;
	SwapChain*       mSwapChain       = nullptr;
	Commandbuffer*   mCommandbuffer   = nullptr;
	Framebuffer*     mFramebuffer     = nullptr;
	GraphicPipeline* mGraphicPipeline = nullptr;

public:	
	VulkanCore(const char* aAppName, int aWidth, int aHeight, HINSTANCE aCurrentInstance, const char* mVertexShader, const char* mFragmentShader);
	~VulkanCore();

	void run();
	void addShader(const char* mVertexShader, const char* mFragmentShader);

private:
	void init(const char* aAppName, int aWidth, int aHeight, HINSTANCE aCurrentInstance, const char* mVertexShader, const char* mFragmentShader);
	void mainLoop();
	void drawFrame();
	void createSyncObjects();
};


#endif // !CORE_H
