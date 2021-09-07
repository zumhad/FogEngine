#include "EditHelper.h"

#include "Application.h"
#include "Devices.h"
#include "Direct3D.h"

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

void Edit::SetSceneRect(int16 left, int16 top, int16 right, int16 bottom)
{
	if (mApp->mIsGame) return;

	SetSceneRectLeft(left);
	SetSceneRectTop(top);
	SetSceneRectRight(right);
	SetSceneRectBottom(bottom);

	mApp->ResizeScene();
}

void Edit::SetCursorEnabled(bool var)
{
	mApp->mMouse->SetEnabled(var);
	mApp->SetCursorState(var);
}

void Edit::SetSceneRect(Rect* rect)
{
	if (mApp->mIsGame) return;

	SetSceneRectLeft(rect->left);
	SetSceneRectTop(rect->top);
	SetSceneRectRight(rect->right);
	SetSceneRectBottom(rect->bottom);

	mApp->ResizeScene();
}

void Edit::SetSceneRectLeft(int16 left)
{
	if (mApp->mIsGame) return;

	mApp->mProperties.sceneRect.left = std::max((int16)0, left);
	mApp->ResizeScene();
}

void Edit::SetSceneRectTop(int16 top)
{
	if (mApp->mIsGame) return;

	mApp->mProperties.sceneRect.top = std::max((int16)0, top);
	mApp->ResizeScene();
}

void Edit::SetSceneRectRight(int16 right)
{
	if (mApp->mIsGame) return;

	mApp->mProperties.sceneRect.right = std::max((int16)0, right);
	mApp->ResizeScene();
}

void Edit::SetSceneRectBottom(int16 bottom)
{
	if (mApp->mIsGame) return;

	mApp->mProperties.sceneRect.bottom = std::max((int16)0, bottom);
	mApp->ResizeScene();
}

void Edit::MoveSceneX(int16 x)
{
	if (mApp->mIsGame) return;
	if (mApp->mProperties.sceneRect.left+x < 0) return;

	SetSceneRectLeft(mApp->mProperties.sceneRect.left + x);
	SetSceneRectRight(mApp->mProperties.sceneRect.right + x);
}

void Edit::MoveSceneY(int16 y)
{
	if (mApp->mIsGame) return;
	if (mApp->mProperties.sceneRect.top + y < 0) return;

	SetSceneRectTop(mApp->mProperties.sceneRect.top + y);
	SetSceneRectBottom(mApp->mProperties.sceneRect.bottom + y);
}


bool Edit::IsAppPaused() { return mApp->mPaused; }
HWND Edit::GetHWND() { return mApp->mHwnd; }
Rect* Edit::GetSceneRect() { return &(mApp->mProperties.sceneRect); }
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

void Edit::SetSceneColorR(int32 red) { SetRValue(mApp->mProperties.sceneColor, red); }
void Edit::SetSceneColorG(int32 green) { SetGValue(mApp->mProperties.sceneColor, green); }
void Edit::SetSceneColorB(int32 blue) { SetBValue(mApp->mProperties.sceneColor, blue); }