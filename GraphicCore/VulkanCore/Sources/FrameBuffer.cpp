#include "../Headers/FrameBuffer.h"

Framebuffer::Framebuffer(VkDevice aDevice, VkRenderPass aRenderPass, VkExtent2D aExtent2D, std::vector<VkImageView>& aSwapChainImageViews, size_t aSwapChainImageViewsSize) : mDevice(aDevice)
{
	createFramebuffers(aRenderPass, aExtent2D, aSwapChainImageViews, aSwapChainImageViewsSize);
}

Framebuffer::~Framebuffer()
{
	for (auto framebuffer : mSwapChainFramebuffers) 
		vkDestroyFramebuffer(mDevice, framebuffer, nullptr);	
}

void Framebuffer::createFramebuffers(VkRenderPass aRenderPass, VkExtent2D aExtent2D, std::vector<VkImageView>& aSwapChainImageViews, size_t aSwapChainImageViewsSize)
{
	mSwapChainFramebuffers.resize(aSwapChainImageViewsSize);

	for (size_t i = 0; i < aSwapChainImageViewsSize; i++)
	{
		VkImageView attachments[] = {aSwapChainImageViews[i]};

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass      = aRenderPass;
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments    = attachments;
		framebufferInfo.width           = aExtent2D.width;
		framebufferInfo.height          = aExtent2D.height;
		framebufferInfo.layers          = 1;

		if(vkCreateFramebuffer(mDevice, &framebufferInfo, nullptr, &mSwapChainFramebuffers[i]) != VK_SUCCESS)
			throw std::runtime_error("failed to create framebuffer!");
	}
}