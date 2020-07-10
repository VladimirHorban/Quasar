#include "../Headers/Window.h"

Window::Window(HINSTANCE aCurrentInstance, const char* aWindowName, int aWidth, int aHeight) : mCurrentInstance(aCurrentInstance), mWindowName(aWindowName), mWidth(aWidth), mHeight(aHeight)
{    
    initWindow();
}

Window::~Window()
{
	
}

void Window::initWindow()
{
    // Register the window class
    const char* CLASS_NAME = mWindowName;
    WNDCLASS wc{};
    wc.hInstance     = mCurrentInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor       = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpfnWndProc   = WindowProcessMessages;
    RegisterClass(&wc);

    // Create the window
    mPHwnd = CreateWindow(CLASS_NAME, "Vulkan window",
            WS_OVERLAPPEDWINDOW | WS_VISIBLE,			// Window style
            CW_USEDEFAULT, CW_USEDEFAULT,				// Window initial position
            mWidth, mHeight,			            	// Window size
            nullptr, nullptr, nullptr, nullptr);    
}

LRESULT CALLBACK WindowProcessMessages(HWND hwnd, UINT msg, WPARAM param, LPARAM lparam)
{
    switch (msg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_ENTERSIZEMOVE:
        std::cout << "Moving or resizing" << std::endl;

    case WM_EXITSIZEMOVE:
        {
            VulkanCore& mCore = VulkanCore::getVulkanCoreInstance();
            mCore.recreateSwapChain();
        }        

    default:
        return DefWindowProc(hwnd, msg, param, lparam);
    }
    
}

void Window::run() 
{
    MSG msg{};
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

LPRECT Window::getWindowRect() 
{
    LPRECT aLPRECT{};
    GetWindowRect(mPHwnd, aLPRECT);
    return aLPRECT;
}