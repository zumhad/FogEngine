#include "ApplicationEngine.h"

#include "Definitions.h"
#include "Direct3D.h"
#include "Devices.h"
#include "Trace.h"
#include "Time.h"
#include "ObjectManager.h"
#include "CameraEngine.h"
#include "Input.h"
#include "GUI.h"

#include <ctime>
#include <shellapi.h>


HWND ApplicationEngine::mHwnd = 0;

Module::Time* Application::mTime = 0;
Module::Keyboard* Application::mKeyboard = 0;
Module::Mouse* Application::mMouse = 0;

bool Application::mIsGame = false;
bool Application::mMinimized = false;
bool Application::mMaximized = false;
bool Application::mResizing = false;
bool Application::mPaused = false;
bool Application::mStarted = false;

void Application::RestartShader() { Direct3D::InitializeShader(); }


void ApplicationEngine::InitWindow()
{
    WCHAR exePath[MAX_PATH];                   //
    GetModuleFileName(0, exePath, MAX_PATH);   // get icon
    HICON hIcon = ExtractIcon(0, exePath, 0);  //

    String::Strcpy(Singlton.path, exePath, 0, String::FindStr(exePath, APP_NAME) + String::Strlen(APP_NAME));

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
        style = WS_POPUP | WS_THICKFRAME | WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;
    }

    mHwnd = CreateWindowEx(0, APP_CLASS, APP_NAME, style, 0, 0, width, height, 0, 0, 0, this);

    if(!mIsGame)
        SetWindowPos(mHwnd, HWND_TOP, 0, 0, width, height, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
}


void ApplicationEngine::InitApp()
{
    mIsGame = Singlton.isGame;

    InitWindow();
    InitModules();

    srand((unsigned int)time(0));   // true random
    if (!mMouse->GetEnabled())
        ShowCursor(false);

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
    Direct3D::Setup();
    mMouse = new Module::Mouse();
    mTime = new Module::Time;
    mKeyboard = new Module::Keyboard;
    CameraEngine::Setup();
    GUI::Setup();
}

void Application::InitBuffers()
{
    CameraEngine::Update(Module::Time::DeltaTime());

	for (int i = 0; i < 2; i++)
	{
		if (mIsGame)
		{
			Direct3D::DrawGame();
			Direct3D::Present();
		}
		else
		{
			Direct3D::DrawEngine();
			Direct3D::Present();
		}
	}
}

int ApplicationEngine::Run()
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

void ApplicationEngine::SetCursorState(bool var)
{
    if (mMouse->GetState() != var)
    {
        mMouse->SetState(var);
        if (mStarted) ShowCursor(var);
    }
}

bool ApplicationEngine::CursorInScene()
{
    POINT p;
    GetCursorPos(&p);
    ScreenToClient(mHwnd, &p);

    RECT r = { Singlton.scene.x, Singlton.scene.y, Singlton.scene.x + Singlton.scene.width, Singlton.scene.y + Singlton.scene.height };
    return PtInRect(&r, p);
}

void ApplicationEngine::DefaultProp()
{
    Singlton.game.width = GetSystemMetrics(SM_CXSCREEN);
    Singlton.game.height = GetSystemMetrics(SM_CYSCREEN);

    Singlton.resolution.width = Singlton.game.width;
    Singlton.resolution.height = Singlton.game.height;

    RECT rect{};
    SystemParametersInfoW(SPI_GETWORKAREA, 0, &rect, 0);
    Singlton.editor.width = rect.right - rect.left;
    Singlton.editor.height = rect.bottom - rect.top;

    Singlton.scene.width = Singlton.game.width / 2;
    Singlton.scene.height = Singlton.game.height / 2;
    Singlton.scene.y = Singlton.captionHeight;
}


ApplicationEngine::~ApplicationEngine() //exit
{
    if (!mMouse->GetEnabled())
        ShowCursor(true);

    DestroyWindow(mHwnd);
    UnregisterClass(APP_CLASS, 0);

    GUI::Shotdown();
    ObjectManager::Shotdown();
    Direct3D::Shotdown();

    SAFE_DELETE(mKeyboard);
    SAFE_DELETE(mTime);
    SAFE_DELETE(mMouse);
}

void Application::SetCursorEnabled(bool var)
{
    mMouse->SetEnabled(var);

    if (mMouse->GetState() != var)
    {
        mMouse->SetState(var);
        if (mStarted) ShowCursor(var);
    }
}

void Application::SetSceneX(int x)
{
    if (mIsGame) return;

    Singlton.scene.x = (int)Math::Max(0.0f, (float)x);

    Direct3D::ResizeScene();
    InitBuffers();
}

void Application::SetSceneY(int y)
{
    if (mIsGame) return;

    Singlton.scene.y = (int)Math::Max(0.0f, (float)y);

    Direct3D::ResizeScene();
    InitBuffers();
}

void Application::SetSceneWidth(int width)
{
    if (mIsGame) return;

    Singlton.scene.width = (int)Math::Max(0.0f, (float)width);

    Direct3D::ResizeScene();
    InitBuffers();
}

void Application::SetSceneHeight(int height)
{
    if (mIsGame) return;

    Singlton.scene.height = (int)Math::Max(0.0f, (float)height);

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


bool Application::CursorInScene()
{
    if (Input::GetCursorX() <= GetSceneX() + GetSceneWidth() &&
        Input::GetCursorX() >= GetSceneX() &&
        Input::GetCursorY() <= GetSceneY() + GetSceneHeight() &&
        Input::GetCursorY() >= GetSceneY())
        return true;

    return false;
}

int Application::GetSceneX() { return Singlton.scene.x; }
int Application::GetSceneY() { return Singlton.scene.y; }
int Application::GetSceneWidth() { return Singlton.scene.width; }
int Application::GetSceneHeight() { return Singlton.scene.height; }

bool Application::IsAppPaused() { return mPaused; }
bool Application::GetCursorEnabled() { return mMouse->GetState(); }

void Application::SetSceneColor(int red, int green, int blue) { Singlton.scene.color = { red, green, blue }; }

void Application::Exit() { PostQuitMessage(0); }