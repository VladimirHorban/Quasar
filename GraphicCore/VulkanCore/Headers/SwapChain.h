#ifndef SWAPCHAIN_H
#define SWAPCHAIN_H

#include <vector>
#include <cstdint>
#include <algorithm>

#include "CommonHeaders.h"
#include "Common.h"

struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR        mCapabilities;
	std::vector<VkSurfaceFormatKHR> mFormats;
	std::vector<VkPresentModeKHR>   mPresentModes;
};

class SwapChain
{
private:
	VkSwapchainKHR mSwapchain;

	const uint32_t           mWidth  = 800;
	const uint32_t           mHeight = 600;

	VkFormat                 mSwapChainFormat;
	VkExtent2D               mSwapChainExtent2D;

	std::vector<VkImage>     mSwapChainImages;
	std::vector<VkImageView> mSwapChainImageViews;

public:
	SwapChain(const VkDevice& aDevice, const VkPhysicalDevice& aPhysicalDevice, const VkSurfaceKHR& aSurface, const QueueFamilyIndices* aQueueFamilyIndices);
	~SwapChain() {}

	static SwapChainSupportDetails querySwapChainSupport(const VkPhysicalDevice& aPhysicalDevice, const VkSurfaceKHR& aSurface);

	inline VkSwapchainKHR const getSwapChain() const
	{
		return mSwapchain;
	}

	inline VkExtent2D getSwapChainExtend() const
	{
		return mSwapChainExtent2D;
	}

	inline VkFormat* getSwapChainFormat()  
	{
		return &mSwapChainFormat;
	}

	inline size_t const getSwapChainImageViewsSize() const 
	{
		return mSwapChainImageViews.size();
	}

	inline std::vector<VkImageView>& const getSwapChainImageViews() 
	{
		return mSwapChainImageViews;
	}

private:
	void               createSwapchain(const VkDevice& aDevice, const VkPhysicalDevice& aPhysicalDevice, const VkSurfaceKHR& aSurface, const QueueFamilyIndices* aQueueFamilyIndices);
	void               createImageViews(const VkDevice& aDevice);

	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> aAvailableFormats);
	VkPresentModeKHR   chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& aAvailablePresentModes);
	VkExtent2D         chooseSwapExtend(const VkSurfaceCapabilitiesKHR& aCapabilities);
};

#endif // !SWAPCHAIN_H
