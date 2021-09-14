#include "Application.h"

#include "Definitions.h"
#include "Direct3D.h"
#include "Devices.h"
#include "Trace.h"
#include "Time.h"
#include "ParseFile.h"

#include <ctime>
#include <shellapi.h>

void Application::InitWindow()
{
    WCHAR szExePath[MAX_PATH];                   //
    GetModuleFileName(0, szExePath, MAX_PATH);   // get icon
    HICON hIcon = ExtractIcon(0, szExePath, 0);  //

    WNDCLASSEX wce = { 0 };
    wce.cbSize = sizeof(WNDCLASSEX);
    wce.style = CS_HREDRAW | CS_VREDRAW;
    wce.lpfnWndProc = WndProc;
    wce.hCursor = LoadCursor(0, IDC_ARROW);
    wce.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wce.lpszClassName = APP_CLASS;
    wce.hIcon = hIcon;

    FOG_ASSERT(RegisterClassEx(&wce));

    DWORD style;
    int width, height;

    if (mIsGame)
    {
        width = Singlton.game.width;
        height = Singlton.game.height;
        style = WS_POPUP;
    }
    else
    {
        width = Singlton.editor.width;
        height = Singlton.editor.height;
        style = WS_POPUP | WS_THICKFRAME | WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;
    }

    mHwnd = CreateWindowEx(0, APP_CLASS, APP_NAME, style, 0, 0, width, height, 0, 0, 0, this);

    if(!mIsGame)
        SetWindowPos(mHwnd, HWND_TOP, 0, 0, width, height, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
}


void Application::InitApp()
{
    mIsGame = Singlton.isGame;

    Edit::SetApplication(this);
    Parse* parse = new Parse();
    SAFE_DELETE(parse);

    InitWindow();
    InitModules();
    InitBuffers();

    srand((unsigned int)time(0));   // true random
    if (!mMouse->GetEnabled())
        ShowCursor(false);

    if (Singlton.foo.start)
        Singlton.foo.start;

    mStarted = true;
}

void Application::InitModules()
{
    mDirect = new Module::Direct3D(mHwnd);
    mMouse = new Module::Mouse();
    mTime = new Module::Time;
    mKeyboard = new Module::Keyboard;
    mCamera = new Module::Camera();
}

void Application::InitBuffers()
{
    if (mIsGame)
    {
        mDirect->DrawGame();
        mDirect->Present();
    }
    else
    {
        for (int i = 0; i < 2; i++)
        {
            mDirect->DrawEngine();
            mDirect->Present();
        }
    }
}

int Application::Run()
{
    InitApp();

    ShowWindow(mHwnd, SW_MAXIMIZE);
    mTime->Reset(); //start timer

    MSG msg = { 0 };
    while (WM_QUIT != msg.message)
    {
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}

void Application::SetCursorState(bool var)
{
    if (mMouse->GetState() != var)
    {
        mMouse->SetState(var);
        if (mStarted) ShowCursor(var);
    }
}

bool Application::CursorInScene()
{
    POINT p;
    GetCursorPos(&p);
    ScreenToClient(mHwnd, &p);

    RECT r = { Singlton.scene.x, Singlton.scene.y, Singlton.scene.x + Singlton.scene.width, Singlton.scene.y + Singlton.scene.height };
    return PtInRect(&r, p);
}

Application::Application()
{
    prop.game.width = (short)GetSystemMetrics(SM_CXSCREEN);
    prop.game.height = (short)GetSystemMetrics(SM_CYSCREEN);

    prop.resolution.width = (short)GetSystemMetrics(SM_CXSCREEN);
    prop.resolution.height = (short)GetSystemMetrics(SM_CYSCREEN);

    RECT rect;
    SystemParametersInfoW(SPI_GETWORKAREA, 0, &rect, 0);
    prop.editor.width = short(rect.right - rect.left);
    prop.editor.height = short(rect.bottom - rect.top);

    prop.scene.width = prop.game.width / 2;
    prop.scene.height = prop.game.height / 2;
}

Application::~Application() //exit
{
    if (!mMouse->GetEnabled())
        ShowCursor(true);

    DestroyWindow(mHwnd);
    UnregisterClass(APP_CLASS, 0);

    SAFE_DELETE(mDirect);
    SAFE_DELETE(mKeyboard);
    SAFE_DELETE(mTime);
    SAFE_DELETE(mCamera);
    SAFE_DELETE(mMouse);
}