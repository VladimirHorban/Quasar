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

class Window;
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
	WindowSurface*   mWindowSurface   = nullptr;
	PhysicalDevice*  mPhysicalDevice  = nullptr;
	LogicalDevice*   mDevice          = nullptr;
	SwapChain*       mSwapChain       = nullptr;
	Commandbuffer*   mCommandbuffer   = nullptr;
	Framebuffer*     mFramebuffer     = nullptr;
	GraphicPipeline* mGraphicPipeline = nullptr;

	// to do
	const char* mVertexShader;
	const char* mFragmentShader;

public:
	void init(const char* aAppName, int aWidth, int aHeight, HINSTANCE aCurrentInstance, const char* aVertexShader, const char* aFragmentShader);
	void run();

	static VulkanCore& const getVulkanCoreInstance()
	{
		static VulkanCore instance;
		return instance;
	}

	void recreateSwapChain();

private:	
	VulkanCore();
	VulkanCore(const VulkanCore&) {}
	VulkanCore& operator= (VulkanCore&) {}
	~VulkanCore();

	void mainLoop();
	void drawFrame();
	void createSyncObjects();
	void cleanupSwapChain();
};


#endif // !CORE_H
