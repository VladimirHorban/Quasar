#ifndef WINDOW_SURFACE_H
#define WINDOW_SURFACE_H

#include <Windows.h>
#include "CommonHeaders.h"
#include "../../ThirdParty/vulkan/vulkan_win32.h"

class WindowSurface
{
private:	
	VkSurfaceKHR mSurface;

public:	
	WindowSurface(const VkInstance& aInstance,const HWND& aHWND,const HINSTANCE& aHINSTANCE);
	~WindowSurface();	

	inline VkSurfaceKHR const getVkSurfaceKHR() const
	{
		return mSurface;
	}	
};

#endif // !WINDOW_SURFACE_H
