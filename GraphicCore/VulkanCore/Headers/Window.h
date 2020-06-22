#ifndef WINDOW_H
#define WINDOW_H

#include <Windows.h>
#include "CommonHeaders.h"

class Window
{
private:	
	const char* mWindowName = "None";

	int         mWidth      = 0;
	int         mHeight     = 0;

	HWND        mPHwnd;
	HINSTANCE   mCurrentInstance;

public:
	Window(HINSTANCE aCurrentInstance, const char* aWindowName, int aWidth, int aHeight);
	~Window();

	void run();

	inline HWND const getPHWND() const
	{
		return mPHwnd;
	}

private:
	void initWindow();	
};

LRESULT CALLBACK WindowProcessMessages(HWND hwnd, UINT msg, WPARAM param, LPARAM lparam);
#endif // !WINDOW_H