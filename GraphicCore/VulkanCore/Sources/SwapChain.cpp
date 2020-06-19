#include "../Headers/SwapChain.h"

SwapChain::SwapChain(const VkDevice& aDevice, const VkPhysicalDevice& aPhysicalDevice, const VkSurfaceKHR& aSurface, const QueueFamilyIndices* aQueueFamilyIndices)
{
	createSwapchain(aDevice, aPhysicalDevice, aSurface, aQueueFamilyIndices);
	createImageViews(aDevice);
}

SwapChainSupportDetails SwapChain::querySwapChainSupport(const VkPhysicalDevice& aPhysicalDevice, const VkSurfaceKHR& aSurface)
{
	SwapChainSupportDetails details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(aPhysicalDevice, aSurface, &details.mCapabilities);

	uint32_t formatCount = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(aPhysicalDevice, aSurface, &formatCount, nullptr);

	if (formatCount != 0)
	{
		details.mFormats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(aPhysicalDevice, aSurface, &formatCount, details.mFormats.data());
	}

	uint32_t presentModeCount = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(aPhysicalDevice, aSurface, &presentModeCount, nullptr);

	if (presentModeCount != 0)
	{
		details.mPresentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(aPhysicalDevice, aSurface, &presentModeCount, details.mPresentModes.data());
	}

	return details;
}

VkSurfaceFormatKHR SwapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> aAvailableFormats)
{
	for (const VkSurfaceFormatKHR& availableFormatIt : aAvailableFormats) 
	{
		if (availableFormatIt.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormatIt.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			return availableFormatIt;		
	}

	return aAvailableFormats[0];
}

VkPresentModeKHR SwapChain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& aAvailablePresentModes)
{
	for (const VkPresentModeKHR& availablePresentModeIt : aAvailablePresentModes) 
	{
		if (availablePresentModeIt == VK_PRESENT_MODE_MAILBOX_KHR)
			return availablePresentModeIt;
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D SwapChain::chooseSwapExtend(const VkSurfaceCapabilitiesKHR& aCapabilities)
{
	if (aCapabilities.currentExtent.width != UINT32_MAX)
		return aCapabilities.currentExtent;

	else 
	{
		VkExtent2D actualExtend = {mWidth , mHeight};

		actualExtend.width = std::max(aCapabilities.maxImageExtent.width, std::min(aCapabilities.maxImageExtent.width, actualExtend.width));
		actualExtend.height = std::max(aCapabilities.maxImageExtent.height, std::min(aCapabilities.maxImageExtent.height, actualExtend.height));

		return actualExtend;
	}
	
}

void SwapChain::createSwapchain(const VkDevice& aDevice, const VkPhysicalDevice& aPhysicalDevice, const VkSurfaceKHR& aSurface, const QueueFamilyIndices* aQueueFamilyIndices)
{
	SwapChainSupportDetails swapChainSupport = querySwapChainSupport(aPhysicalDevice, aSurface);

	VkSurfaceFormatKHR surfaseFormat = chooseSwapSurfaceFormat(swapChainSupport.mFormats);
	VkPresentModeKHR   presentMode   = chooseSwapPresentMode(swapChainSupport.mPresentModes);
	VkExtent2D         extent        = chooseSwapExtend(swapChainSupport.mCapabilities);

	uint32_t imageCount = swapChainSupport.mCapabilities.minImageCount + 1;

	if (swapChainSupport.mCapabilities.maxImageCount > 0 && imageCount > swapChainSupport.mCapabilities.maxImageCount) 
		imageCount = swapChainSupport.mCapabilities.maxImageCount;

	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType                    = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface                  = aSurface;
	createInfo.minImageCount            = imageCount;
	createInfo.imageFormat              = surfaseFormat.format;
	createInfo.imageColorSpace          = surfaseFormat.colorSpace;
	createInfo.imageExtent              = extent;
	createInfo.imageArrayLayers         = 1;
	createInfo.imageUsage               = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	uint32_t queueFamilyIndices[] = { aQueueFamilyIndices->mGraphicsFamily.value(), aQueueFamilyIndices->mPresentFamily.value()};

	if (aQueueFamilyIndices->mGraphicsFamily != aQueueFamilyIndices->mPresentFamily)
	{
		createInfo.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices   = queueFamilyIndices;
	}
	else 
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;

	createInfo.preTransform   = swapChainSupport.mCapabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode    = presentMode;
	createInfo.clipped        = VK_TRUE;
	createInfo.oldSwapchain   = VK_NULL_HANDLE;

	if(vkCreateSwapchainKHR(aDevice, &createInfo, nullptr, &mSwapchain) != VK_SUCCESS)
		throw std::runtime_error("failed to create swap chain!");

	vkGetSwapchainImagesKHR(aDevice, mSwapchain, &imageCount, nullptr);
	mSwapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(aDevice, mSwapchain, &imageCount, mSwapChainImages.data());

	mSwapChainFormat   = surfaseFormat.format;
	mSwapChainExtent2D = extent;
}

void SwapChain::createImageViews(const VkDevice& aDevice)
{
	size_t swapChainImagesSize = mSwapChainImages.size();
	mSwapChainImageViews.resize(swapChainImagesSize);

	for (int i = 0; i < swapChainImagesSize; i++)
	{
		VkImageViewCreateInfo createInfo           = {};
		createInfo.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image                           = mSwapChainImages[i];
		createInfo.viewType                        = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format                          = mSwapChainFormat;
		createInfo.components.r                    = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g                    = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b                    = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a                    = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel   = 0;
		createInfo.subresourceRange.levelCount     = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount     = 1;		

		if (vkCreateImageView(aDevice, &createInfo, nullptr, &mSwapChainImageViews[i]) != VK_SUCCESS)
			throw std::runtime_error("failed to create image views!");
	}	
}
