#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <fstream>
#include <vector>
#include <stdexcept>
#include "../../ThirdParty/vulkan/vulkan.h"

class ShaderManager
{
public:
	ShaderManager()  {}
	~ShaderManager() {}

	static VkShaderModule createShaderModule(const VkDevice& aDevice, const char* aFileName);

private:	
	static std::vector<char> readFile(const VkDevice& aDevice, const char* aFileName);
};

#endif // !SHADERMANAGER_H
