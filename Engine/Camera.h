#pragma once

#include "Core.h"

#include "Properties.h"
#include "MathHelper.h"

using namespace Math;

namespace Module
{
	class Camera
	{
	public:
		Camera();

		DirectX::XMMATRIX GetViewMatrix() { return mView; }
		void Update(float dt);
		void SetPosition(Vector3 position) { mPosition = position; mTargetPosition = position; }
		void SetRotation(float x, float y, float z);
		void SetRotationX(float x) { mPitch = x; mTargetRotation = DirectX::XMQuaternionRotationRollPitchYaw(-mPitch, mYaw, mRoll); }
		void SetRotationY(float y) { mYaw = y; mTargetRotation = DirectX::XMQuaternionRotationRollPitchYaw(-mPitch, mYaw, mRoll); }
		void SetRotationZ(float z) { mRoll = z; mTargetRotation = DirectX::XMQuaternionRotationRollPitchYaw(-mPitch, mYaw, mRoll); }

		void MoveLocal(float x, float y, float z);
		void MoveGlobal(float x, float y, float z);
		void SetTarget(Vector3 target);
		void LookAt();
		void Rotate(float x, float y, float z);
		float GetRotateX() { return mPitch; };
		float GetRotateY() { return mYaw; };
		float GetRotateZ() { return mRoll; };

	private:
		void Initialize(Vector3 eye, Vector3 at);

	private:
		float mYaw;
		float mPitch;
		float mRoll;

		float mRotationSmooth;
		float mMoveSmooth;

		float mDotDistance;
		Vector3 mDotPosition;

		Vector3 mPosition;
		Vector3 mTargetPosition;
		Quaternion mRotation;
		Quaternion mTargetRotation;

		Matrix4 mView;
	};
}