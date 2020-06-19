// Quasar.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <Windows.h>
#include "GraphicCore/VulkanCore/Headers/Validation.h"
#include "GraphicCore/VulkanCore/Headers/Instance.h"
#include "GraphicCore/VulkanCore/Headers/PhysicalDevice.h"
#include "GraphicCore/VulkanCore/Headers/WindowSurface.h"
#include "GraphicCore/VulkanCore/Headers/LogicalDevice.h"
#include "GraphicCore/VulkanCore/Headers/Window.h"
#include "GraphicCore/VulkanCore/Headers/SwapChain.h"
#include "GraphicCore/VulkanCore/Headers/Pipeline.h"
#include "GraphicCore/VulkanCore/Headers/FrameBuffer.h"
#include "GraphicCore/VulkanCore/Headers/CommandBuffer.h"

class TestApp 
{
private:
	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence>     inFlightFences;
	std::vector<VkFence>     imagesInFlight;
	size_t currentFrame      = 0;

	const int MAX_FRAMES_IN_FLIGHT = 2;
	
	LogicalDevice*   mDevice          = nullptr;
	SwapChain*       mSwapChain       = nullptr;
	Commandbuffer*   mCommandbuffer   = nullptr;
	GraphicPipeline* mGraphicPipeline = nullptr;

public:
	TestApp(LogicalDevice* aLogicalDevice, SwapChain* aSwapChain, Commandbuffer* aCommandbuffer, GraphicPipeline* aGraphicPipeline) : mDevice(aLogicalDevice), mSwapChain(aSwapChain), mCommandbuffer(aCommandbuffer), mGraphicPipeline(aGraphicPipeline)
	{}

	~TestApp() {}

	void run();

private:
	void mainLoop();
	void drawFrame();
	void createSyncObjects();
};


