#pragma once

#include "Core.h"

#include "Object.h"
#include "MathHelper.h"
using namespace Math;



class Application;

class FOG_API Edit
{
public:
	static void SetApplication(Application* app) { mApp = app; }
	static void CheckDebug();

	static void Exit();
	static void SetCursorEnabled(bool var);

	static void SetSceneRect(int16 left, int16 top, int16 right, int16 bottom);
	static void SetSceneRect(Rect* rect);
	static void SetSceneRectLeft(int16 left);
	static void SetSceneRectTop(int16 top);
	static void SetSceneRectRight(int16 right);
	static void SetSceneRectBottom(int16 bottom);

	static void SetSceneColorR(int32 red);
	static void SetSceneColorG(int32 green);
	static void SetSceneColorB(int32 blue);

	static void MoveSceneX(int16 x);
	static void MoveSceneY(int16 y);

	static bool IsAppPaused();
	static HWND	 GetHWND();
	static Rect* GetSceneRect();
	static bool GetCursorEnabled();

	static void CameraSetTarget(Vector3 target);
	static void CameraRotate(float x, float y, float z);
	static void CameraLookAt();
	static void CameraSetRotation(float x, float y, float z);
	static float CameraGetRotateX();
	static float CameraGetRotateY();
	static float CameraGetRotateZ();
	static void CameraSetRotationX(float x);
	static void CameraSetRotationY(float y);
	static void CameraSetRotationZ(float z);
	static void CameraMoveLocal(float x, float y, float z);
	static void CameraMoveGlobal(float x, float y, float z);
	
private:
	static Application* mApp;
};