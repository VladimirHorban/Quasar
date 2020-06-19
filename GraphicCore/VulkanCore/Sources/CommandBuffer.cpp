#include "../Headers/CommandBuffer.h"

Commandbuffer::Commandbuffer(const VkDevice* aDevice, const VkPipeline* aGraphicPipeline,VkRenderPass* aRenderPass, const Framebuffer* aFramebuffer, const VkExtent2D* aExtent2D, const QueueFamilyIndices* aFamilyIndices) : mDevice(aDevice)
{
	createCommandPool(aFamilyIndices);
	createCommandBuffers(aGraphicPipeline, aRenderPass, aFramebuffer, aExtent2D);
}

Commandbuffer::~Commandbuffer()
{
	vkDestroyCommandPool(*mDevice, mCommandPool, nullptr);
}

void Commandbuffer::createCommandPool(const QueueFamilyIndices* aFamilyIndices)
{
	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = (*aFamilyIndices).mGraphicsFamily.value();
	poolInfo.flags            = 0; 

	if(vkCreateCommandPool(*mDevice, &poolInfo, nullptr, &mCommandPool) != VK_SUCCESS)
		throw std::runtime_error("failed to create command pool!");
}

void Commandbuffer::createCommandBuffers(const VkPipeline* aGraphicPipeline, const VkRenderPass* aRenderPass, const Framebuffer* aFramebuffer, const VkExtent2D* aExtent2D)
{
	mCommandBuffers.resize(aFramebuffer->getSwapChainFramebuffersSize());

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool        = mCommandPool;
	allocInfo.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t)mCommandBuffers.size();

	if(vkAllocateCommandBuffers(*mDevice, &allocInfo, mCommandBuffers.data()) != VK_SUCCESS)
		throw std::runtime_error("failed to allocate command buffers!");

	for (size_t i = 0; i < mCommandBuffers.size(); i++)
	{
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags            = 0; 
		beginInfo.pInheritanceInfo = nullptr; 

		if (vkBeginCommandBuffer(mCommandBuffers[i], &beginInfo) != VK_SUCCESS)
			throw std::runtime_error("failed to begin recording command buffer!");
		
		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = *aRenderPass;
		renderPassInfo.framebuffer = aFramebuffer->getSwapChainFramebuffers()[i];
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = *aExtent2D;

		VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		vkCmdBeginRenderPass(mCommandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		vkCmdBindPipeline(mCommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, *aGraphicPipeline);

		vkCmdDraw(mCommandBuffers[i], 3, 1, 0, 0);

		vkCmdEndRenderPass(mCommandBuffers[i]);

		if (vkEndCommandBuffer(mCommandBuffers[i]) != VK_SUCCESS)
			throw std::runtime_error("failed to record command buffer!");
	}		
}