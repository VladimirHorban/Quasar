#include "../Headers/CoreMinimal.h"

CoreMinimal::CoreMinimal(const char* aAppName, int aWidth, int aHeight, HINSTANCE aCurrentInstance, const char* mVertexShader, const char* mFragmentShader)
{
	mVulkanCore.init(aAppName, aWidth, aHeight, aCurrentInstance, mVertexShader, mFragmentShader);
}

void CoreMinimal::run() 
{
	mVulkanCore.run();
}
