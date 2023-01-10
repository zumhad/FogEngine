#pragma once

#include "Core.h"

#include "Matrix.h"
#include "Vector3.h"

#include <DirectXMath.h>

class FOG_API Camera
{
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

protected:
	static Vector3 mPosition;
	static Vector3 mRotation;

	static Matrix mView;
	static Matrix mProj;

	static float mWidth;
	static float mHeight;
	static float mFOV;
	static float mAspectRatio;
	static float mNearZ;
	static float mFarZ;
	static bool m3D;
};