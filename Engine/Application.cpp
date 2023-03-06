#include "Application.h"

#include "Definitions.h"
#include "Direct3D.h"
#include "Trace.h"
#include "Timer.h"
#include "ObjectManager.h"
#include "Camera.h"
#include "Input.h"
#include "GUI.h"
#include "Cursor.h"
#include "FrustumCulling.h"
#include "MathHelper.h"
#include "Text.h"

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
WCHAR Application::mPath[MAX_PATH] = {};
int Application::mCaptionHeight = 0;
int Application::mFpsMax = 0;

decltype(Application::mScene) Application::mScene = {};
decltype(Application::mEditor) Application::mEditor = {};
decltype(Application::mGame) Application::mGame = {};
decltype(Application::mFoo) Application::mFoo = {};

void Application::InitWindow()
{
    WCHAR exePath[MAX_PATH];                   //
    GetModuleFileName(0, exePath, MAX_PATH);   // get icon
    HICON hIcon = ExtractIcon(0, exePath, 0);  //

    String::Strcpy(mPath, exePath, 0, String::FindStr(exePath, L"\\FogEngine") + String::Strlen(L"\\FogEngine"));

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
        width = GetSystemMetrics(SM_CXSCREEN);
        height = GetSystemMetrics(SM_CYSCREEN);
        style = WS_POPUP;
    }
    else
    {
        width = mEditor.width;
        height = mEditor.height;
        style = WS_OVERLAPPEDWINDOW;
    }

    mHwnd = CreateWindowEx(0, APP_CLASS, APP_NAME, style, 0, 0, width, height, 0, 0, 0, 0);
}

void Application::InitProp(APPCLASS app)
{
    mIsGame = app.isGame;
    mCaptionHeight = (int)Math::Max(0.0f, (float)app.captionHeight);
    mFpsMax = app.fpsMax;
    mFoo.start = app.foo.start;
    mFoo.update = app.foo.update;

    RECT rect{};
    SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);

    mEditor.width = rect.right - rect.left;
    mEditor.height = rect.bottom - rect.top;
    mEditor.color = app.editor.color;

    mGame.width = app.game.width;
    mGame.height = app.game.height;
    mGame.color = app.game.color;

    mScene.x = app.scene.x;
    mScene.y = app.scene.y;
    mScene.width = app.scene.width;
    mScene.height = app.scene.height;
    mScene.color = app.scene.color;
}

void Application::InitApp(APPCLASS app)
{
    CheckDebug();
    srand((unsigned int)time(0));

    InitProp(app);
    InitWindow();
    InitModules(app);

    if (app.foo.start)
        app.foo.start();

    InitBuffers();
    mStarted = true;
}

void Application::CheckDebug()
{
#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
}

void Application::InitModules(APPCLASS app)
{
    Direct3D::Setup();
    Input::Setup();
    GUI::Setup();
    Text::Setup();
    Time::Setup();
    Camera::Setup(app);
    ObjectManager::Setup();
    FrustumCulling::Setup();
}

void Application::InitBuffers()
{
    Camera::Update();

	for (int i = 0; i < 2; i++)
	{
		if (mIsGame)
			Direct3D::DrawGame();
		else
			Direct3D::DrawEngine();

        Direct3D::Present();
	}
}

int Application::Run(APPCLASS app)
{
	InitApp(app);

	Cursor::SetVisible(app.cursorShow);
	ShowWindow(mHwnd, SW_MAXIMIZE);
	Time::Reset(); //start timer

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


void Application::Shotdown() //exit
{
    DestroyWindow(mHwnd);
    UnregisterClass(APP_CLASS, 0);

    FrustumCulling::Shotdown();
    ObjectManager::Shotdown();
    Text::Shotdown();
    GUI::Shotdown();
    Direct3D::Shotdown();
}

void Application::SetSceneX(int x)
{
    if (mIsGame) return;

    mScene.x = (int)Math::Max(0.0f, float(x));

    Direct3D::ResizeScene();
}

void Application::SetSceneY(int y)
{
    if (mIsGame) return;

    mScene.y = (int)Math::Max(0.0f, float(y));

    Direct3D::ResizeScene();
}

void Application::SetSceneWidth(int width)
{
    if (mIsGame) return;

    mScene.width = (int)Math::Max(0.0f, float(width));

    Direct3D::ResizeScene();
}

void Application::SetSceneHeight(int height)
{
    if (mIsGame) return;

    mScene.height = (int)Math::Max(0.0f, float(height));

    Direct3D::ResizeScene();
}

void Application::SetSceneColor(Color color)
{ 
    if (mIsGame) return;

    mScene.color = color;
}

int Application::GetSceneX() 
{
    if (mIsGame) return 0;

    return mScene.x; 
}

int Application::GetSceneY()
{
    if (mIsGame) return 0;

    return mScene.y;
}

int Application::GetSceneWidth()
{
    if (mIsGame) return 0;

    return mScene.width;
}

int Application::GetSceneHeight()
{
    if (mIsGame) return 0;

    return mScene.height;
}

Color Application::GetSceneColor() 
{
    if (mIsGame) return 0;

    return mScene.color;
}

void Application::SetEditorColor(Color color)
{
    if (mIsGame) return;

    mEditor.color = color;
}

int Application::GetEditorWidth()
{
    if (mIsGame) return 0;

    return mEditor.width;
}

int Application::GetEditorHeight()
{
    if (mIsGame) return 0;

    return mEditor.height;
}

Color Application::GetEditorColor()
{
    if (mIsGame) return 0;

    return mEditor.color;
}

void Application::SetGameWidth(int width)
{
    if (!mIsGame) return;

    mGame.width = width;

    Direct3D::ResizeGame();
}

void Application::SetGameHeight(int height)
{
    if (!mIsGame) return;

    mGame.height = height;

    Direct3D::ResizeGame();
}

void Application::SetGameColor(Color color)
{
    if (!mIsGame) return;

    mGame.color = color;
}

int Application::GetGameWidth()
{
    if (!mIsGame) return 0;

    return mGame.width;
}

int Application::GetGameHeight()
{
    if (!mIsGame) return 0;

    return mGame.height;
}

Color Application::GetGameColor()
{
    if (!mIsGame) return 0;

    return mGame.color;
}

int Application::GetFpsMax() { return mFpsMax; }

void Application::SetFpsMax(int fps) { mFpsMax = fps; }

int Application::GetCaptionHeight() { return mCaptionHeight; }

void Application::SetCaptionHeight(int height) { mCaptionHeight = height; }

void Application::Restore()
{ 
    if (mMaximized)
        ShowWindow(mHwnd, SW_RESTORE);
    else
        ShowWindow(mHwnd, SW_MAXIMIZE);
}

void Application::Close() { PostQuitMessage(0); }
void Application::Minimize() { ShowWindow(mHwnd, SW_MINIMIZE); }
bool Application::IsPaused() { return mPaused; }
WCHAR* Application::GetPath() { return mPath; }
bool Application::IsGame() { return mIsGame; }
