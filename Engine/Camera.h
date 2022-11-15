#pragma once

#include "Core.h"

#include "Properties.h"
#include "MathHelper.h"
#include "Frustum.h"

using namespace Math;

class FOG_API Camera
{
public:
	static void SetPosition(Vector3 position);
	static void SetRotation(Vector3 rotation);
	static void SetRotation(float x, float y, float z);
	static void SetRotationX(float x);
	static void SetRotationY(float y);
	static void SetRotationZ(float z);

	static void LookAt(Vector3 pos);
	static void MoveLocal(float x, float y, float z);
	static void MoveGlobal(float x, float y, float z);
	static void Rotate(float x, float y, float z);
	static float GetRotateX() { return mRotation.GetX(); };
	static float GetRotateY() { return mRotation.GetY(); };
	static float GetRotateZ() { return mRotation.GetZ(); };

	static Vector3 GetRotation() { return mRotation; }
	static Vector3 GetPosition() { return mPosition; }

protected:
	static void UpdateProjMatrix();

protected:
	static float mYaw;
	static float mPitch;
	static float mRoll;

	static float mRotationSmooth;
	static float mMoveSmooth;

	static Vector3 mPosition;
	static Vector3 mRotation;
	static Vector3 mTargetPosition;
	static Vector3 mTargetRotation;
	static OrthogonalTransform mToWorld;
	static OrthogonalTransform mTargetToWorld;

	static Matrix4 mWorld;
	static Matrix4 mView;
	static Matrix4 mProj;
	static Matrix4 mViewProj;

	static Frustum mFrustumVS;
	static Frustum mFrustumWS;

	static float mWidth;
	static float mHeight;
	static float mFOV;
	static float mAspectRatio;
	static float mNearZ;
	static float mFarZ;
	static bool m3D;
};