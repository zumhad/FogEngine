#pragma once

#include "Core.h"

#include "Properties.h"
#include "EditHelper.h"
#include "Time.h"


namespace Module
{
	class Time;
	class Direct3D;
	class Mouse;
	class Keyboard;
	class Camera;
}


class FOG_API Application	
{
public:
	Application() {}
	~Application();
	int32 Run();

protected:
	ENGINECLASS mProperties;

private:
	friend void Edit::SetSceneRect(int16 left, int16 top, int16 right, int16 bottom);
	friend void Edit::SetCursorEnabled(bool var);
	friend void Edit::SetSceneRect(Rect* rect);
	friend void Edit::SetSceneRectLeft(int16 left);
	friend void Edit::SetSceneRectTop(int16 top);
	friend void Edit::SetSceneRectRight(int16 right);
	friend void Edit::SetSceneRectBottom(int16 bottom);
	friend void Edit::SetSceneColorR(int32 red);
	friend void Edit::SetSceneColorG(int32 green);
	friend void Edit::SetSceneColorB(int32 blue);
	friend void Edit::MoveSceneX(int16 x);
	friend void Edit::MoveSceneY(int16 y);
	friend HWND Edit::GetHWND();
	friend Rect* Edit::GetSceneRect();
	friend bool Edit::GetCursorEnabled();
	friend bool Edit::IsAppPaused();
	friend void Edit::CameraRotate(float x, float y, float z);
	friend void Edit::CameraSetTarget(Vector3 target);
	friend void Edit::CameraLookAt();
	friend void Edit::CameraSetRotation(float x, float y, float z);
	friend float Edit::CameraGetRotateX();
	friend float Edit::CameraGetRotateY();
	friend float Edit::CameraGetRotateZ();
	friend void Edit::CameraSetRotationX(float x);
	friend void Edit::CameraSetRotationY(float y);
	friend void Edit::CameraSetRotationZ(float z);
	friend void Edit::CameraMoveLocal(float x, float y, float z);
	friend void Edit::CameraMoveGlobal(float x, float y, float z);

private:
	void InitApp();
	void InitWindow();
	void InitModules(ENGINECLASS* ec);
	void InitModules(GAMECLASS* gc);

	void AdjustMaxClient(RECT& rect);
	void ResizeScene();
	void Update();
	void CalculateFrameStats();
	LRESULT HitTest();
	void SetCursorState(bool var);
	bool CursorInScene();


	static LRESULT CALLBACK EditorProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

private:
	Time* mTime = 0;
	Module::Keyboard* mKeyboard = 0;
	Module::Direct3D* mDirect = 0;
	Module::Camera* mCamera = 0;
	Module::Mouse* mMouse = 0;

	HWND mHwnd = 0;

	bool mMinimized = false;
	bool mMaximized = false;
	bool mResizing = false;
	bool mPaused = false;
	bool mStarted = false;

	bool mIsGame = false;

	void (*pHandleKey)() = 0;
};