#ifndef LOGICAL_DEVICE
#define LOGICAL_DEVICE

#include <set>
#include <vector>

#include "CommonHeaders.h"
#include "PhysicalDevice.h"
#include "WindowSurface.h"

class LogicalDevice
{
private:
	VkDevice mDevice;
	VkQueue  mGraphicsQueue;
	VkQueue  mPresentQueue;

public:
	LogicalDevice(PhysicalDevice& aPhysicalDevice, const WindowSurface& aWindowSurface);	
	~LogicalDevice();

	inline VkDevice* getDevice()
	{
		return &mDevice;
	}

	inline VkQueue* getGraphicsQueue()
	{
		return &mGraphicsQueue;
	}

	inline VkQueue* getPresentQueue() 
	{
		return &mPresentQueue;
	}

private:
	void createLogicalDevice(PhysicalDevice& aPhysicalDevice, const WindowSurface& aWindowSurface);
	void createGraphicsQueue(PhysicalDevice& aPhysicalDevice, const WindowSurface& aWindowSurface);
};

#endif // !LOGICAL_DEVICE
