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

	static void SetSceneX(int16);
	static void SetSceneY(int16);
	static void SetSceneWidth(int16);
	static void SetSceneHeight(int16);

	static void SetSceneColor(int16 red, int16 green, int16 blue);

	static void MoveSceneX(int16 x);
	static void MoveSceneY(int16 y);

	static bool IsAppPaused();
	static HWND	 GetHWND();
	static bool GetCursorEnabled();
	static int16 GetSceneX();
	static int16 GetSceneY();
	static int16 GetSceneWidth();
	static int16 GetSceneHeight();

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