#ifndef WINDOW_H
#define WINDOW_H

#include <Windows.h>
#include "CommonHeaders.h"
#include "../../ThirdParty/vulkan/vulkan_win32.h"

class Window
{
private:	
	const char* mWindowName = "None";

	int         mWidth      = 0;
	int         mHeight     = 0;

public:
	Window(const char* aWindowName, int aWidth, int aHeight);
	~Window();

private:
	void initWindow();	
};

#endif // !WINDOW_H