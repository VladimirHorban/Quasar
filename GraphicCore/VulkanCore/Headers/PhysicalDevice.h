#ifndef PHYSICAL_DEVICE
#define PHYSICAL_DEVICE

#include <vector>
#include <map>
#include <string>
#include <set>

#include "Common.h"
#include "CommonHeaders.h"
#include "Instance.h"
#include "WindowSurface.h"
#include "SwapChain.h"

const std::vector<const char*> mDeviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

class PhysicalDevice
{
private:
	VkPhysicalDevice   mPhysicalDevice = VK_NULL_HANDLE;
	QueueFamilyIndices mQueueFamilyIndices;

public:
	PhysicalDevice(const VkInstance& aInstance, const VkSurfaceKHR& aWindowSurface);
	~PhysicalDevice();

	inline VkPhysicalDevice& const getVkPhysicalDevice() 
	{
		return mPhysicalDevice;
	}

	inline QueueFamilyIndices* const getQueueFamilyIndices() 
	{
		return &mQueueFamilyIndices;
	}

private:
	void pickPhysicalDevice(const VkInstance& aInstance, const VkSurfaceKHR& aWindowSurface);
	bool isDeviceSuitable(const VkPhysicalDevice& aDevice, const VkSurfaceKHR& aWindowSurface);
	bool checkDeviceExtensionsSupport(const VkPhysicalDevice& aDevice);
	int  rateDeviceSuitability(VkPhysicalDevice device);	

	QueueFamilyIndices const findQueueFamilies(const VkPhysicalDevice& aDevice, const VkSurfaceKHR& aWindowSurface);
};

#endif // !PHYSICAL_DEVICE
