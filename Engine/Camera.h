#pragma once

#include "Core.h"

#include "Properties.h"
#include "MathHelper.h"
#include "Frustum.h"

using namespace Math;

class FOG_API Camera
{
public:
	static void SetPosition(Vector3 position) { mToWorld.SetTranslation(position); mTargetToWorld.SetTranslation(position); }
	static void SetRotation(Vector3 rotation);
	static void SetRotation(float x, float y, float z);
	static void SetRotationX(float x);
	static void SetRotationY(float y);
	static void SetRotationZ(float z);

	static void LookAt(Vector3 pos);
	static void MoveLocal(float x, float y, float z);
	static void MoveGlobal(float x, float y, float z);
	static void Rotate(float x, float y, float z);
	static float GetRotateX() { return mPitch; };
	static float GetRotateY() { return mYaw; };
	static float GetRotateZ() { return mRoll; };

	static Vector3 GetRotation() { return Vector3(mPitch, mYaw, mRoll); }
	static Vector3 GetPosition() { return mToWorld.GetTranslation(); }

	static void SetPerspective(float verticalFovRadians, float aspectHeightOverWidth, float nearZClip, float farZClip);

private:
	static void UpdateProjMatrix();

protected:
	static float mYaw;
	static float mPitch;
	static float mRoll;

	static float mRotationSmooth;
	static float mMoveSmooth;

	static OrthogonalTransform mToWorld;
	static OrthogonalTransform mTargetToWorld;

	static Matrix4 mView;
	static Matrix4 mProj;
	static Matrix4 mViewProj;

	static Frustum mFrustumVS;
	static Frustum mFrustumWS;

private:
	static float mVerticalFOV;
	static float mAspectRatio;
	static float mNearClip;
	static float mFarClip;
};