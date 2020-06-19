#include "../Headers/LogicalDevice.h"

LogicalDevice::LogicalDevice(PhysicalDevice& aPhysicalDevice, const WindowSurface& aWindowSurface)
{
	createLogicalDevice(aPhysicalDevice, aWindowSurface);
	createGraphicsQueue(aPhysicalDevice, aWindowSurface);
}

LogicalDevice::~LogicalDevice()
{
	vkDestroyDevice(mDevice, nullptr);
}

void LogicalDevice::createLogicalDevice(PhysicalDevice& aPhysicalDevice, const WindowSurface& aWindowSurface)
{	
	QueueFamilyIndices indices = *aPhysicalDevice.getQueueFamilyIndices();

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t>                   uniqueQueueFamilies = {indices.mGraphicsFamily.value(), indices.mPresentFamily.value()};

	float queuePriority = 1.0f;

	for (uint32_t queueFamilyIt : uniqueQueueFamilies) 
	{
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamilyIt;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}	
	   
	VkPhysicalDeviceFeatures deviceFeatures = {};

	VkDeviceCreateInfo deviceInfo      = {};
	deviceInfo.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceInfo.queueCreateInfoCount    = static_cast<uint32_t>(queueCreateInfos.size());
	deviceInfo.pQueueCreateInfos       = queueCreateInfos.data();
	deviceInfo.pEnabledFeatures        = &deviceFeatures;

	deviceInfo.enabledExtensionCount   = static_cast<uint32_t>(mDeviceExtensions.size());
	deviceInfo.ppEnabledExtensionNames = mDeviceExtensions.data();

	/*if (enableValidationLayers) 
	{
		deviceInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		deviceInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else
	{
		deviceInfo.enabledLayerCount = 0;
	}*/

	if (vkCreateDevice(aPhysicalDevice.getVkPhysicalDevice(), &deviceInfo, nullptr, &mDevice) != VK_SUCCESS)
		throw std::runtime_error("failed to create logical device!");
}

void LogicalDevice::createGraphicsQueue(PhysicalDevice& aPhysicalDevice, const WindowSurface& aWindowSurface)
{
	QueueFamilyIndices indices = *aPhysicalDevice.getQueueFamilyIndices();

	vkGetDeviceQueue(mDevice, indices.mGraphicsFamily.value(), 0, &mGraphicsQueue);
	vkGetDeviceQueue(mDevice, indices.mPresentFamily.value(), 0, &mPresentQueue);
}