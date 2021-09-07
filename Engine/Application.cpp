#include "Application.h"

#include "ParseFile.h"
#include "EditHelper.h"
#include "PathHelper.h"
#include "Devices.h"
#include "Direct3D.h"
#include "Time.h"

#include <ctime>
#include <algorithm>
#include <string>
#include "shellapi.h"

Application::~Application() //exit
{
	/*if (!mMouse->GetEnabled())
		ShowCursor(false);

	DestroyWindow(mHwnd);
	UnregisterClass(APP_CLASS, 0);

	SAFE_DELETE(mDirect);
	SAFE_DELETE(mKeyboard);
	SAFE_DELETE(mTime);
	SAFE_DELETE(mCamera);
	SAFE_DELETE(mMouse);*/
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

	RECT r = { mProperties.sceneRect.left, mProperties.sceneRect.top, mProperties.sceneRect.right, mProperties.sceneRect.bottom };
	return PtInRect(&r, p);
}

void Application::InitModules(ENGINECLASS*) //engine
{
	if (mIsGame)
	{
		mCamera = new Module::Camera(&mProperties.camera);
		mDirect = new Module::Direct3D(mHwnd, mProperties.gameRect, mProperties.gameColor, &mProperties.camera);
		mMouse = new Module::Mouse(mProperties.showCursor, mIsGame);
		mTime = new Time(mProperties.maxFps);
		mKeyboard = new Module::Keyboard();
	}
	else
	{
		mCamera = new Module::Camera(&mProperties.camera);
		mDirect = new Module::Direct3D(mHwnd, mProperties.editorRect, mProperties.editorColor, mProperties.sceneRect, mProperties.sceneColor, &mProperties.camera);
		mMouse = new Module::Mouse(mProperties.showCursor, mIsGame);
		mTime = new Time(mProperties.maxFps);
		mKeyboard = new Module::Keyboard();
	}
}

void Application::InitWindow()
{
	WCHAR szExePath[MAX_PATH];                   //
	GetModuleFileName(0, szExePath, MAX_PATH);   // get icon
	HICON hIcon = ExtractIcon(0, szExePath, 0);  //

	WNDCLASS wce = {};
	wce.hCursor = LoadCursor(0, IDC_ARROW);
	wce.lpszClassName = APP_CLASS;
	wce.lpfnWndProc = EditorProc;
	wce.hIcon = hIcon;
	wce.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);

	RegisterClass(&wce);

	DWORD style;
	int32 x, y;
	Rect rect;
	if (mIsGame)
	{ 
		rect = mProperties.gameRect;
		style = WS_POPUP;
		x = 0;
		y = 0;
	}
	else
	{
		rect = mProperties.editorRect;
		style = WS_POPUP | WS_THICKFRAME | WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;
		x = CW_USEDEFAULT;
		y = CW_USEDEFAULT;
	}

	mHwnd = CreateWindowEx(0 ,APP_CLASS, APP_NAME, style, x, y, RECT_WIDTH(rect), RECT_HEIGHT(rect), 0, 0, 0, this);

	if(!mIsGame)
		SetWindowPos(mHwnd, HWND_TOP, x, y, RECT_WIDTH(mProperties.editorRect), RECT_HEIGHT(mProperties.editorRect), SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
	SetForegroundWindow(mHwnd);
	SetFocus(mHwnd);
}

void Application::ResizeScene()
{
	if (mIsGame) return;

	if (mStarted)
	{
		mDirect->ResizeScene(mProperties.sceneRect.left, mProperties.sceneRect.top, mProperties.sceneRect.right, mProperties.sceneRect.bottom);
		mDirect->DrawEngine();
		mDirect->Present();
	}
}


void Application::Update()
{
	float dt = Time::DeltaTime();

	mMouse->Update();

	mCamera->Update(dt);

	mDirect->UpdateViewMatrix(mCamera->GetViewMatrix());
	mDirect->Update(dt);
}

void Application::InitApp()
{
	mIsGame = mProperties.isGame;

	Edit::SetApplication(this);
	Parse* parse = new Parse();
	SAFE_DELETE(parse);

	InitWindow();
	InitModules(&mProperties);

	srand((uint32)time(0));   // true random
	if (!mMouse->GetEnabled())
		ShowCursor(false);

	mTime->Reset(); //start timer
	mStarted = true;
}

int32 Application::Run()
{
	InitApp();

	if (mProperties.init)
		mProperties.init();

	if (mIsGame)
	{
		mDirect->DrawGame();
		mDirect->Present();
	}
	else
	{
		for (uint8 i = 0; i < 2; i++)
		{
			mDirect->DrawEngine();
			mDirect->Present();
		}
	}

	ShowWindow(mHwnd, SW_SHOW);

	MSG msg{};
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int32)msg.wParam;
}

void Application::CalculateFrameStats()
{
	static int frameCnt = 0;
	static float timeElapsed = 0.0f;
	static std::wstring caption;

	frameCnt++;

	if ((mTime->TotalTime() - timeElapsed) >= 1.0f)
	{
		caption = std::to_wstring(frameCnt).c_str();
		SetWindowText(mHwnd, caption.c_str());

		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}