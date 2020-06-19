#ifndef COMMANDBUFFER_H
#define COMMANDBUFFER_H

#include <vector>

#include "CommonHeaders.h"
#include "Common.h"
#include "FrameBuffer.h"

class Commandbuffer
{
private:
	VkCommandPool                mCommandPool;
	std::vector<VkCommandBuffer> mCommandBuffers;

	const VkDevice*              mDevice = nullptr;

public:
	Commandbuffer(const VkDevice* aDevice, const VkPipeline* aGraphicPipeline,VkRenderPass* aRenderPass, const Framebuffer* aFramebuffer, const VkExtent2D* aExtent2D, const QueueFamilyIndices* aFamilyIndices);
	~Commandbuffer();

	inline std::vector<VkCommandBuffer> getCommandBuffers()
	{
		return mCommandBuffers;
	}

private:
	void createCommandPool(const QueueFamilyIndices* aFamilyIndices);
	void createCommandBuffers(const VkPipeline* aGraphicPipeline, const VkRenderPass* aRenderPass, const Framebuffer* aFramebuffer,const VkExtent2D* aExtent2D);
};

#endif // !COMMANDBUFFER_H
