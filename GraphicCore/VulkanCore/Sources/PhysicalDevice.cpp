#include "../Headers/PhysicalDevice.h"

PhysicalDevice::PhysicalDevice(const VkInstance& aInstance, const VkSurfaceKHR& aWindowSurface)
{
	pickPhysicalDevice(aInstance, aWindowSurface);
}

PhysicalDevice::~PhysicalDevice()
{
}

bool PhysicalDevice::isDeviceSuitable(const VkPhysicalDevice& aDevice, const VkSurfaceKHR& aWindowSurface)
{
	// to do
	QueueFamilyIndices indices = findQueueFamilies(aDevice, aWindowSurface);
	mQueueFamilyIndices = indices;

	bool extensionsSupported = checkDeviceExtensionsSupport(aDevice);

	bool swapChainAdequate   = false;
	if (extensionsSupported)
	{
		SwapChainSupportDetails supportDetailes = SwapChain::querySwapChainSupport(aDevice, aWindowSurface);
		swapChainAdequate = !supportDetailes.mFormats.empty() && !supportDetailes.mPresentModes.empty();
	}

	return indices.isComplete() && extensionsSupported && swapChainAdequate;
}

void PhysicalDevice::pickPhysicalDevice(const VkInstance& aInstance, const VkSurfaceKHR& aWindowSurface)
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(aInstance, &deviceCount, nullptr);

	if (deviceCount == 0) 
		throw std::runtime_error("failed to find GPUs with Vulkan support!");

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(aInstance, &deviceCount, devices.data());

	std::multimap<int, VkPhysicalDevice> candidates;

	for (const VkPhysicalDevice& deviceIt : devices) 
	{
		if (isDeviceSuitable(deviceIt, aWindowSurface))
		{
			int score = rateDeviceSuitability(deviceIt);
			candidates.insert(std::make_pair(score, deviceIt));
		}		
	}

	if (candidates.rbegin()->first > 0) 
		mPhysicalDevice = candidates.rbegin()->second;	
	else 
		throw std::runtime_error("failed to find a suitable GPU!");	
}

int PhysicalDevice::rateDeviceSuitability(VkPhysicalDevice device) 
{
	VkPhysicalDeviceProperties physicalDeviceProperties;
	VkPhysicalDeviceFeatures   physicalDeviceFeatures;

	vkGetPhysicalDeviceProperties(device, &physicalDeviceProperties);
	vkGetPhysicalDeviceFeatures(device, &physicalDeviceFeatures);

	int score = 0;

	if (physicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		score += 1000;

	score += physicalDeviceProperties.limits.maxImageDimension2D;

	if (!physicalDeviceFeatures.geometryShader)
		return 0;
}

QueueFamilyIndices const PhysicalDevice::findQueueFamilies(const VkPhysicalDevice& aDevice, const VkSurfaceKHR& aWindowSurface)
{
	QueueFamilyIndices indices;
	VkBool32           presentSupport = false;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(aDevice, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);

	vkGetPhysicalDeviceQueueFamilyProperties(aDevice, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const VkQueueFamilyProperties& queueFamilyIt : queueFamilies) 
	{
		if (queueFamilyIt.queueFlags & VK_QUEUE_GRAPHICS_BIT) 
		{
			indices.mGraphicsFamily = i;
		}
		
		vkGetPhysicalDeviceSurfaceSupportKHR(aDevice, i, aWindowSurface, &presentSupport);

		if(presentSupport)
			indices.mPresentFamily = i;

		if (indices.isComplete())
			break;

		i++;
	}

	return indices;
}

bool PhysicalDevice::checkDeviceExtensionsSupport(const VkPhysicalDevice& aDevice)
{
	uint32_t extensionCount = 0;

	vkEnumerateDeviceExtensionProperties(aDevice, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);

	vkEnumerateDeviceExtensionProperties(aDevice, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(mDeviceExtensions.begin(), mDeviceExtensions.end());

	for (const VkExtensionProperties& extensionIt : availableExtensions)
		requiredExtensions.erase(extensionIt.extensionName);

	return requiredExtensions.empty();
}