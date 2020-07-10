#ifndef CORE_MINNIMAL_H
#define CORE_MINNIMAL_H

#include "../../GraphicCore/VulkanCore/Headers/Core.h"

class CoreMinimal
{
private:
	VulkanCore& mVulkanCore = VulkanCore::getVulkanCoreInstance();	

public:
	CoreMinimal(const char* aAppName, int aWidth, int aHeight, HINSTANCE aCurrentInstance, const char* mVertexShader, const char* mFragmentShader);
	~CoreMinimal() {}

	void run();	
};

#endif // !CORE_MINNIMAL_H

