#include "EditHelper.h"

#include "Application.h"
#include "Devices.h"
#include "Direct3D.h"
#include "Input.h"

#include <algorithm>
#include <cstdlib>

#ifdef _DEBUG  //memory debug
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

Application* Edit::mApp = 0;


void Edit::Exit() { PostQuitMessage(EXIT_SUCCESS); }

void Edit::CheckDebug()
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
}

/*void Edit::SetSceneRect(int16 left, int16 top, int16 right, int16 bottom)
{
	if (mApp->mIsGame) return;

	SetSceneRectLeft(left);
	SetSceneRectTop(top);
	SetSceneRectRight(right);
	SetSceneRectBottom(bottom);

	mApp->ResizeScene();
}*/

void Edit::SetCursorEnabled(bool var)
{
	mApp->mMouse->SetEnabled(var);
	mApp->SetCursorState(var);
}

void Edit::SetSceneX(short x)
{
	if (mApp->mIsGame) return;

	Singlton.scene.x = (short)Math::Max(0, x);

	mApp->mDirect->ResizeScene();
	mApp->InitBuffers();
}

void Edit::SetSceneY(short y)
{
	if (mApp->mIsGame) return;

	Singlton.scene.y = (short)Math::Max(0, y);

	mApp->mDirect->ResizeScene();
	mApp->InitBuffers();
}

void Edit::SetSceneWidth(short width)
{
	if (mApp->mIsGame) return;

	Singlton.scene.width = (short)Math::Max(0, width);

	mApp->mDirect->ResizeScene();
	mApp->InitBuffers();
}

void Edit::SetSceneHeight(short height)
{
	if (mApp->mIsGame) return;

	Singlton.scene.height = (short)Math::Max(0, height);

	mApp->mDirect->ResizeScene();
	mApp->InitBuffers();
}

void Edit::MoveSceneX(short x)
{
	if (Singlton.scene.x + x < 0) return;

	SetSceneX(Singlton.scene.x + x);
}

void Edit::MoveSceneY(short y)
{
	if (Singlton.scene.y + y < 0) return;

	SetSceneY(Singlton.scene.y + y);
}

bool Edit::CursorInScene()
{
	if (Input::GetCursorX() <= GetSceneX() + GetSceneWidth() && 
		Input::GetCursorX() >= GetSceneX() &&
		Input::GetCursorY() <= GetSceneY() + GetSceneHeight() &&
		Input::GetCursorY() >= GetSceneY())
		return true;

	return false;
}

short Edit::GetSceneX() { return Singlton.scene.x; }
short Edit::GetSceneY() { return Singlton.scene.y; }
short Edit::GetSceneWidth() { return Singlton.scene.width; }
short Edit::GetSceneHeight() { return Singlton.scene.height; }

bool Edit::IsAppPaused() { return mApp->mPaused; }
HWND Edit::GetHWND() { return mApp->mHwnd; }
bool Edit::GetCursorEnabled() { return mApp->mMouse->GetState(); }

void Edit::CameraSetTarget(Vector3 target) { mApp->mCamera->SetTarget(target); }
void Edit::CameraRotate(float x, float y, float z) { mApp->mCamera->Rotate(x, y, z); }
void Edit::CameraSetRotation(float x, float y, float z) { mApp->mCamera->SetRotation(x, y, z); }
void Edit::CameraLookAt() { mApp->mCamera->LookAt(); }
float Edit::CameraGetRotateX() { return mApp->mCamera->GetRotateX(); }
float Edit::CameraGetRotateY() { return mApp->mCamera->GetRotateX(); }
float Edit::CameraGetRotateZ() { return mApp->mCamera->GetRotateX(); }
void Edit::CameraSetRotationX(float x) { mApp->mCamera->SetRotationX(x); }
void Edit::CameraSetRotationY(float y) { mApp->mCamera->SetRotationY(y); }
void Edit::CameraSetRotationZ(float z) { mApp->mCamera->SetRotationZ(z); }
void Edit::CameraMoveLocal(float x, float y, float z) { mApp->mCamera->MoveLocal(x, y, z); }
void Edit::CameraMoveGlobal(float x, float y, float z) { mApp->mCamera->MoveGlobal(x, y, z); }

void Edit::SetSceneColor(short red, short green, short blue) { Singlton.scene.color = { red, green, blue }; }