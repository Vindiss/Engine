#include "Window.h"
#include "Engine.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx12.h"
#include "ImGui/imgui.h"
#include "Modules/Render/Renderer.h"
#include "ImGui/imgui_internal.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Module::Display::Window::~Window() {
    PostQuitMessage(0);
}

bool Module::Display::Window::Init() 
{
    hInstance = Engine::GetInstance()->GetHInstance();

    return PrepareWindow(width, height);
}

bool Module::Display::Window::Start() 
{
    return MakeWindow();
}

bool Module::Display::Window::PrepareWindow(const int _width,const int _height) 
{
    if (fullscreen)
    {
        HMONITOR hmon = MonitorFromWindow(windowHandler, MONITOR_DEFAULTTONEAREST);
        MONITORINFO mi = { sizeof(mi) };
        GetMonitorInfo(hmon, &mi);

        width = mi.rcMonitor.right - mi.rcMonitor.left;
        height = mi.rcMonitor.bottom - mi.rcMonitor.top;
    }
    else 
    {
        width = _width;
        height = _height;
    }

    windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.style = CS_CLASSDC;
    windowClass.lpfnWndProc = WndProc;
    windowClass.cbClsExtra = NULL;
    windowClass.cbWndExtra = NULL;
    windowClass.hInstance = hInstance;
    windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
    windowClass.lpszMenuName = NULL;
    windowClass.lpszClassName = windowName;
    windowClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&windowClass))
    {
        MessageBox(NULL, L"Error registering class", L"Error", MB_OK | MB_ICONERROR);
        return false;
    }
    
    return true;
}

bool Module::Display::Window::MakeWindow() 
{
    windowHandler = CreateWindowExW(WS_EX_NOREDIRECTIONBITMAP,
        windowName, 
        windowTitle,
        WS_OVERLAPPEDWINDOW | WS_MAXIMIZE,
        CW_USEDEFAULT, CW_USEDEFAULT,
        width, height,
        NULL,
        NULL,
        hInstance,
        NULL);

    if (!windowHandler)
    {
        MessageBox(NULL, L"Error creating window",
            L"Error", MB_OK | MB_ICONERROR);
        return false;
    }

    if (fullscreen)
    {
        SetWindowLong(windowHandler, GWL_STYLE, 0);
    }

    ShowWindow(windowHandler, SW_SHOWDEFAULT);
    UpdateWindow(windowHandler);

    return true;
}


LRESULT CALLBACK Module::Display::Window::WndProc(HWND _window_h, UINT _window_msg, WPARAM _window_param, LPARAM _long_param)
{
    if (Engine::GetInstance()->IsRunning())
    {
        Module::Display::Window* window = Engine::GetInstance()->GetModule<Module::Display::Window>();

        if (ImGui_ImplWin32_WndProcHandler(_window_h, _window_msg, _window_param, _long_param))
            return true;

        switch (_window_msg)
        {
        case WM_DESTROY:
            Engine::GetInstance()->Stop();
            return 0;

        case WM_SIZE:
        {
            Engine::GetInstance()->GetModule<Module::Render::Renderer>()->GetRenderer()->WindowResize(_window_param, _long_param);
            window->width = (UINT)LOWORD(_long_param);
            window->height = (UINT)HIWORD(_long_param);
            return 0;
        }

        default:
            return DefWindowProc(_window_h, _window_msg, _window_param, _long_param);
        }
    }
    
    return DefWindowProc(_window_h, _window_msg, _window_param, _long_param);
}


void Module::Display::Window::GetMessages() 
{
    MSG msg;
    ZeroMemory(&msg, sizeof(MSG));

    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT) {
            Engine::GetInstance()->Stop();
        }
            
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

}

bool Module::Display::Window::Update()
{
    GetMessages();

    return true;
}

bool Module::Display::Window::Destruct() 
{
    if (windowHandler)
    {
        const bool result = DestroyWindow(windowHandler) != 0;
        windowHandler = nullptr;
        return result;
    }

    PostQuitMessage(0);

    return true;
}

