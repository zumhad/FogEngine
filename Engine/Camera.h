#pragma once

#include "Core.h"

#include "Application.h"
#include "Matrix.h"
#include "Vector3.h"

#include <DirectXMath.h>

class Application;
class ObjectManager;
class Cursor;

class FOG_API Camera
{
	friend class Application;
	friend class ObjectManager;
	friend class Cursor;

public:
	static void SetPosition(Vector3 position);
	static void SetPosition(float x, float y, float z);
	static void SetPositionX(float x);
	static void SetPositionY(float y);
	static void SetPositionZ(float z);

	static void SetRotation(Vector3 rotation);
	static void SetRotation(float x, float y, float z);
	static void SetRotationX(float x);
	static void SetRotationY(float y);
	static void SetRotationZ(float z);

	static void LookAt(Vector3 pos);
	static void Move(Vector3 v);
	static void Rotate(Vector3 v);
	static float GetRotateX() { return mRotation.x; };
	static float GetRotateY() { return mRotation.y; };
	static float GetRotateZ() { return mRotation.z; };

	static Vector3 GetPosition() { return mPosition; }
	static Vector3 GetRotation() { return mRotation; }
	static Vector3 GetDirection();

	static void SetFOV(float fov);
	static void SetNear(float nearZ);
	static void SetFar(float farZ);

private:
	static Matrix GetViewMatrix() { return mView; }
	static Matrix GetProjMatrix() { return mProj; }
	static void Setup(APPCLASS& app);
	static void Update();

private:
	static Vector3 mPosition;
	static Vector3 mRotation;

	static Matrix mView;
	static Matrix mProj;

	static float mFOV;
	static float mAspectRatio;
	static float mNearZ;
	static float mFarZ;
};