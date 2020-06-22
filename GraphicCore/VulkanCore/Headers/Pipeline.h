#ifndef PIPELINE_H
#define PIPELINE_H

#include "CommonHeaders.h"
#include "../../SPIRV-Tools/Headers/ShaderManager.h"

#include <vector>

class GraphicPipeline
{
private:
	VkDevice*        mDevice          = nullptr;
	VkExtent2D*      mSwapChainExtent = nullptr;
	VkFormat*        mSwapChainFormat = nullptr;
	VkPipelineLayout mPipelineLayout;
	VkRenderPass     mRenderPass;

	VkPipeline       mGraphicsPipeline;

	const char* mVertexShader   = nullptr;
	const char* mFragmentShader = nullptr;

public:
	GraphicPipeline(VkDevice* aDevice, VkExtent2D* aSwapChainExtent, VkFormat* aSwapChainFormat, const char* aVertexShader, const char* aFragmentShader);
	~GraphicPipeline() {}

	inline VkPipeline* const getGraphicPipeline()
	{
		return &mGraphicsPipeline;
	}

	inline VkRenderPass* const getVkRenderPass() 
	{
		return &mRenderPass;
	}

private:
	void createRenderPass();
	void createGraphicsPipeline();

	std::vector<char> readFile(const VkDevice& aDevice, const char* aFileName);
	VkShaderModule createShaderModule(const VkDevice& aDevice, const char* aFileName);
};

#endif // !PIPELINE_H
