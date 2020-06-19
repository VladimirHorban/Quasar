#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <vector>
#include "CommonHeaders.h"

class Framebuffer
{
private:
	std::vector<VkFramebuffer> mSwapChainFramebuffers;

	VkDevice mDevice;

public:
	Framebuffer(VkDevice aDevice, VkRenderPass aRenderPass, VkExtent2D aExtent2D, std::vector<VkImageView>& aSwapChainImageViews, size_t aSwapChainImageViewsSize);
	~Framebuffer();

	inline size_t const getSwapChainFramebuffersSize() const
	{
		return mSwapChainFramebuffers.size();
	}

	inline std::vector<VkFramebuffer> getSwapChainFramebuffers() const
	{
		return mSwapChainFramebuffers;
	}

private:
	void createFramebuffers(VkRenderPass aRenderPass, VkExtent2D aExtent2D, std::vector<VkImageView>& aSwapChainImageViews, size_t aSwapChainImageViewsSize);
};

#endif // !FRAMEBUFFER_H
