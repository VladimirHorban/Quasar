#include "../Headers/WindowSurface.h"

WindowSurface::WindowSurface(const VkInstance& aInstance,const HWND& aHWND,const HINSTANCE& aHINSTANCE)
{
    VkWin32SurfaceCreateInfoKHR createInfo = {};

    createInfo.sType     = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    createInfo.hwnd      = aHWND;
    createInfo.hinstance = aHINSTANCE;

    if (vkCreateWin32SurfaceKHR(aInstance, &createInfo, nullptr, &mSurface) != VK_SUCCESS)
        throw std::runtime_error("failed to create window surface!");
}

WindowSurface::~WindowSurface() {}