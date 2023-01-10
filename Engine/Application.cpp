#include "ApplicationEngine.h"

#include "Definitions.h"
#include "Direct3D.h"
#include "Devices.h"
#include "Trace.h"
#include "TimerEngine.h"
#include "ObjectManager.h"
#include "CameraEngine.h"
#include "InputEngine.h"
#include "GUI.h"
#include "Cursor.h"
#include "FrustumCulling.h"

#include <ctime>
#include <shellapi.h>
#include <crtdbg.h>
#include <algorithm>

HWND Application::mHwnd = 0;

bool Application::mIsGame = false;
bool Application::mMinimized = false;
bool Application::mMaximized = false;
bool Application::mResizing = false;
bool Application::mPaused = false;
bool Application::mStarted = false;

ApplicationEngine& ApplicationEngine::Get()
{ 
    static ApplicationEngine app;
    return app;
}

void ApplicationEngine::InitWindow()
{
    WCHAR exePath[MAX_PATH];                   //
    GetModuleFileName(0, exePath, MAX_PATH);   // get icon
    HICON hIcon = ExtractIcon(0, exePath, 0);  //

    String::Strcpy(Singlton.path, exePath, 0, String::FindStr(exePath, L"\\FogEngine") + String::Strlen(L"\\FogEngine"));

    WNDCLASSEX wce{};
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
        style = WS_OVERLAPPEDWINDOW;
    }

    mHwnd = CreateWindowEx(0, APP_CLASS, APP_NAME, style, 0, 0, width, height, 0, 0, 0, 0);

    //if(!mIsGame)
        //SetWindowPos(mHwnd, HWND_TOP, 0, 0, width, height, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
}


void ApplicationEngine::InitApp()
{
    srand((unsigned int)time(0));

    InitWindow();
    InitModules();

    if (Singlton.foo.start)
        Singlton.foo.start();

    InitBuffers();
    mStarted = true;
}

void ApplicationEngine::CheckDebug()
{
#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
}

void ApplicationEngine::InitModules()
{
    InputEngine::Setup();
    Direct3D::Setup();
    TimeEngine::Setup();
    CameraEngine::Setup();
    GUI::Setup();
    ObjectManager::Setup();
    FrustumCulling::Setup();
}

void Application::InitBuffers()
{
    CameraEngine::Update();

	for (int i = 0; i < 2; i++)
	{
		if (mIsGame)
			Direct3D::DrawGame();
		else
			Direct3D::DrawEngine();

        Direct3D::Present();
	}
}

int ApplicationEngine::Run()
{
    InitApp();

    Cursor::SetVisible(Singlton.cursorShow);
    ShowWindow(mHwnd, SW_MAXIMIZE);
    TimeEngine::Reset(); //start timer

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

bool Application::CursorInScene()
{
    int x = Cursor::GetPosition(CURSOR_X);
    int y = Cursor::GetPosition(CURSOR_Y);
    POINT p = { x, y };

    int left = GetSceneX();
    int top = GetSceneY();
    int right = GetSceneX() + GetSceneWidth();
    int bootom = GetSceneX() + GetSceneHeight();

    RECT r = { left, top, right, bootom };
    return PtInRect(&r, p);
}

void ApplicationEngine::DefaultProp()
{
    mIsGame = Singlton.isGame;

    Singlton.game.width = GetSystemMetrics(SM_CXSCREEN);
    Singlton.game.height = GetSystemMetrics(SM_CYSCREEN);

    Singlton.resolution.width = Singlton.game.width;
    Singlton.resolution.height = Singlton.game.height;

    RECT rect{};
    SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);
    Singlton.editor.width = rect.right - rect.left;
    Singlton.editor.height = rect.bottom - rect.top;

    Singlton.scene.width = Singlton.game.width / 2;
    Singlton.scene.height = Singlton.game.height / 2;
    Singlton.scene.y = Singlton.captionHeight;
}


void ApplicationEngine::Shotdown() //exit
{
    DestroyWindow(mHwnd);
    UnregisterClass(APP_CLASS, 0);

    FrustumCulling::Shotdown();
    ObjectManager::Shotdown();
    GUI::Shotdown();
    ObjectManager::Shotdown();
    Direct3D::Shotdown();
}

void Application::SetSceneX(int x)
{
    if (mIsGame) return;

    Singlton.scene.x = std::max(0, x);

    Direct3D::ResizeScene();
    InitBuffers();
}

void Application::SetSceneY(int y)
{
    if (mIsGame) return;

    Singlton.scene.y = std::max(0, y);

    Direct3D::ResizeScene();
    InitBuffers();
}

void Application::SetSceneWidth(int width)
{
    if (mIsGame) return;

    Singlton.scene.width = std::max(0, width);

    Direct3D::ResizeScene();
    InitBuffers();
}

void Application::SetSceneHeight(int height)
{
    if (mIsGame) return;

    Singlton.scene.height = std::max(0, height);

    Direct3D::ResizeScene();
    InitBuffers();
}


void Application::MoveSceneX(int x)
{
    if (Singlton.scene.x + x < 0) return;

    SetSceneX(Singlton.scene.x + x);
}

void Application::MoveSceneY(int y)
{
    if (Singlton.scene.y + y < 0) return;

    SetSceneY(Singlton.scene.y + y);
}

int Application::GetSceneX() { return Singlton.scene.x; }
int Application::GetSceneY() { return Singlton.scene.y; }
int Application::GetSceneWidth() { return Singlton.scene.width; }
int Application::GetSceneHeight() { return Singlton.scene.height; }

bool Application::IsAppPaused() { return mPaused; }

void Application::SetSceneColor(float r, float g, float b) { Singlton.scene.color = Color(r, g, b); }

void Application::Exit() { PostQuitMessage(0); }

void ApplicationEngine::AdjustMaxClient(RECT& rect)
{
    WINDOWPLACEMENT placement = {};
    GetWindowPlacement(mHwnd, &placement);

    if (placement.showCmd != SW_MAXIMIZE) return;

    HMONITOR monitor = MonitorFromWindow(mHwnd, MONITOR_DEFAULTTONULL);

    MONITORINFO monitor_info{};
    monitor_info.cbSize = sizeof(monitor_info);

    if (!GetMonitorInfoW(monitor, &monitor_info)) return;

    rect = monitor_info.rcWork;
}

LRESULT ApplicationEngine::HitTest()
{
    static int xBorder = GetSystemMetrics(SM_CXFRAME) + GetSystemMetrics(SM_CXPADDEDBORDER);
    static int yBorder = GetSystemMetrics(SM_CYFRAME) + GetSystemMetrics(SM_CXPADDEDBORDER);
    int x = Cursor::GetPosition(CURSOR_X);
    int y = Cursor::GetPosition(CURSOR_Y);

    int result =
        BIT_1 * (x < xBorder) |
        BIT_2 * (x >= (Singlton.editor.width - xBorder)) |
        BIT_3 * (y < (xBorder)) |
        BIT_4 * (y >= (Singlton.editor.height - yBorder));

    switch (result)
    {
        case BIT_1: return HTLEFT;
        case BIT_2: return HTRIGHT;
        case BIT_3: return HTTOP;
        case BIT_4: return HTBOTTOM;
        case BIT_3 | BIT_1: return HTTOPLEFT;
        case BIT_3 | BIT_2: return HTTOPRIGHT;
        case BIT_4 | BIT_1: return HTBOTTOMLEFT;
        case BIT_4 | BIT_2: return HTBOTTOMRIGHT;
        default: return ((y <= Singlton.captionHeight) ? HTCAPTION : HTCLIENT);
    }
}
