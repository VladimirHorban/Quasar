#ifndef INSTANCE_H
#define INSTANCE_H

#define VK_USE_PLATFORM_WIN32_KHR

#include <vector>
#include "CommonHeaders.h"
#include "Validation.h"

class Instance
{
private:
/*#ifndef NDEBUG
	const bool mEnableValidationLayers = false;
#else
	const bool mEnableValidationLayers = true;
#endif */
	const bool mEnableValidationLayers = true;
	VkInstance mInstance;
	
	const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };

public:
	Instance(const char* aAppName, const char* aEngineName, int aMajorVersion, int aMinorVersion, int aPatch);	
	~Instance() {}

	inline VkInstance const getVkInstance() const
	{
		return mInstance;
	}

private:
	Instance() {}	
	bool checkValidationLayerSupport();
};


#endif // !INSTANCE_H
