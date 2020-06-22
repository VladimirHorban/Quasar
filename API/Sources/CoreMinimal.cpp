#include "../Headers/CoreMinimal.h"

CoreMinimal::CoreMinimal(const char* aAppName, int aWidth, int aHeight, HINSTANCE aCurrentInstance, const char* mVertexShader, const char* mFragmentShader)
{
	mVulkanCore = new VulkanCore(aAppName, aWidth, aHeight, aCurrentInstance, mVertexShader, mFragmentShader);
}

CoreMinimal::~CoreMinimal()
{
	delete mVulkanCore;
}

void CoreMinimal::run() 
{
	mVulkanCore->run();
}
