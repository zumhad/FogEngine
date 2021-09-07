#include "Camera.h"

#include "Input.h"
#include "Time.h"
#include "Trace.h"

using namespace DirectX;

namespace Module
{
	Camera::Camera(CAMERACLASS* camera)
	{
		mRotationSmooth = camera->rotationSmooth;
		mMoveSmooth = camera->moveSmooth;

		mPosition = camera->position;
		mTargetPosition = camera->position;

		SetTarget(camera->targetPosition);

		Vector3 direction = mDotPosition - mTargetPosition;
		Scalar directionLenSq = LengthSq(direction);
		FOG_ASSERT(directionLenSq != 0.0f);
		direction = direction * RecipSqrt(directionLenSq);

		mPitch = ASin(direction.GetY());
		mYaw = ATan2(direction.GetX(), direction.GetZ());
		mRoll = 0;

		mTargetRotation = XMQuaternionRotationRollPitchYaw(-mPitch, mYaw, mRoll);
		mRotation = mTargetRotation;
	}

	void Camera::SetRotation(float x, float y, float z)
	{
		mPitch = x; mYaw = y; mRoll = z;

		mTargetRotation = XMQuaternionRotationRollPitchYaw(-mPitch, mYaw, mRoll);
	}

	void Camera::Rotate(float x, float y, float z)
	{
		mPitch += x; mYaw += y; mRoll += z;

		mTargetRotation = XMQuaternionRotationRollPitchYaw(-mPitch, mYaw, mRoll);
	}

	void Camera::MoveLocal(float x, float y, float z)
	{
		Vector3 offset = Vector3(XMVector3Rotate(Vector3(x, y, z) * Time::DeltaTime(), mTargetRotation));

		mTargetPosition = offset + mTargetPosition;
	}

	void Camera::MoveGlobal(float x, float y, float z)
	{
		Vector3 offset = Vector3(x, y, z) * Time::DeltaTime();

		mTargetPosition = offset + mTargetPosition;
	}


	void Camera::SetTarget(Vector3 target)
	{
		mDotPosition = target;
		mDotDistance = Length(mDotPosition - mTargetPosition);
	}

	void Camera::LookAt()
	{
		mTargetPosition = mDotPosition - Vector3(XMVector3Rotate(Vector3(0, 0, mDotDistance), mTargetRotation));
	}

	void Camera::Update(float dt)
	{
		mRotation = Lerp(mRotation, mTargetRotation, dt * mRotationSmooth);
		mPosition = Lerp(mPosition, mTargetPosition, dt * mMoveSmooth);

		Quaternion invRotation = ~mRotation;
		mView = Matrix4(Matrix3(invRotation), invRotation * -mPosition);
	}
}

