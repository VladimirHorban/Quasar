#include "../Headers/ShaderManager.h"

std::vector<char> ShaderManager::readFile(const VkDevice& aDevice, const char* aFileName)
{    
    std::ifstream file(aFileName, std::ios::ate | std::ios::binary);

    if (!file.is_open())
    {
        throw std::runtime_error("failed to open file!");
    }

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}

VkShaderModule ShaderManager::createShaderModule(const VkDevice& aDevice, const char* aFileName)
{
    std::vector<char> shaderCode = readFile(aDevice, aFileName);
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = shaderCode.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(shaderCode.data());

    VkShaderModule shaderModule;
    if(vkCreateShaderModule(aDevice, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
        throw std::runtime_error("failed to create shader module!");
}