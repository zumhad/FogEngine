#include "Camera.h"
#include "CameraEngine.h"

#include "Input.h"
#include "Time.h"
#include "Trace.h"

using namespace DirectX;
using namespace Math;
using namespace Module;



float Camera::mYaw;
float Camera::mPitch;
float Camera::mRoll;
float Camera::mRotationSmooth;
float Camera::mMoveSmooth;

OrthogonalTransform Camera::mToWorld;
OrthogonalTransform Camera::mTargetToWorld;

Matrix4 Camera::mView;
Matrix4 Camera::mProj;
Matrix4 Camera::mViewProj;

Frustum Camera::mFrustumVS;
Frustum Camera::mFrustumWS;

float Camera::mVerticalFOV;
float Camera::mAspectRatio;
float Camera::mNearClip;
float Camera::mFarClip;

void Camera::LookAt(Vector3 pos)
{
	pos = Normalize(pos - mTargetToWorld.GetTranslation());

	mPitch = ASin(pos.GetY());
	mYaw = ATan2(pos.GetX(), pos.GetZ());
	mRoll = 0;

	mTargetToWorld.SetRotation(XMQuaternionRotationRollPitchYaw(-mPitch, mYaw, mRoll));
}

void Camera::SetPerspective(float verticalFovRadians, float aspectHeightOverWidth, float nearZClip, float farZClip)
{
	mVerticalFOV = verticalFovRadians * XM_PI / 180.0f;
	mAspectRatio = aspectHeightOverWidth;
	mNearClip = nearZClip;
	mFarClip = farZClip;

	UpdateProjMatrix();
}

void Camera::UpdateProjMatrix()
{
    float Y = 1.0f / tanf(mVerticalFOV * 0.5f);
    float X = Y * mAspectRatio;

	float Q1 = mFarClip / (mFarClip - mNearClip);
	float Q2 = -Q1 * mNearClip;

    mProj = Matrix4(
        Vector4(X, 0.0f, 0.0f, 0.0f),
        Vector4(0.0f, Y, 0.0f, 0.0f),
        Vector4(0.0f, 0.0f, Q1, 1.0f),
        Vector4(0.0f, 0.0f, Q2, 0.0f)
    );

	mFrustumVS = Frustum(mProj);
}


void CameraEngine::Setup()
{
	mRotationSmooth = Singlton.camera.rotationSmooth;
	mMoveSmooth = Singlton.camera.moveSmooth;

	Vector3 forward = Vector3::OneZ();
	Vector3 right = Cross(Vector3::OneY(), forward);
	Vector3 up = Cross(forward, right);

	mPitch = ASin(forward.GetY());
	mYaw = ATan2(forward.GetX(), forward.GetZ());
	mRoll = 0;

	mToWorld.SetRotation(XMQuaternionRotationRollPitchYaw(-mPitch, mYaw, mRoll));
	mToWorld.SetTranslation(Vector3::Zero());

	mTargetToWorld = mToWorld;
}

void Camera::SetRotation(Vector3 rotation)
{
	mPitch = rotation.GetX(); mYaw = rotation.GetY(); mRoll = rotation.GetZ();

	mToWorld.SetRotation(XMQuaternionRotationRollPitchYaw(-mPitch, mYaw, mRoll));
	mTargetToWorld = mToWorld;
}


void Camera::SetRotationX(float x)
{ 
	mPitch = x; 
	mToWorld.SetRotation(XMQuaternionRotationRollPitchYaw(-mPitch, mYaw, mRoll));
	mTargetToWorld = mToWorld;
}
void Camera::SetRotationY(float y)
{ 
	mYaw = y; 
	mToWorld.SetRotation(XMQuaternionRotationRollPitchYaw(-mPitch, mYaw, mRoll));
	mTargetToWorld = mToWorld;
}
void Camera::SetRotationZ(float z) 
{ 
	mRoll = z; 
	mToWorld.SetRotation(XMQuaternionRotationRollPitchYaw(-mPitch, mYaw, mRoll));
	mTargetToWorld = mToWorld;
}


void Camera::SetRotation(float x, float y, float z)
{
	mPitch = x; mYaw = y; mRoll = z;

	mToWorld.SetRotation(XMQuaternionRotationRollPitchYaw(-mPitch, mYaw, mRoll));
	mTargetToWorld = mToWorld;
}

void Camera::Rotate(float x, float y, float z)
{
	mPitch += x; mYaw += y; mRoll += z;

	mTargetToWorld.SetRotation(XMQuaternionRotationRollPitchYaw(-mPitch, mYaw, mRoll));
}

void Camera::MoveLocal(float x, float y, float z)
{
	Vector3 offset = Vector3(XMVector3Rotate(Vector3(x, y, z) * Time::DeltaTime(), mTargetToWorld.GetRotation()));

	mTargetToWorld.SetTranslation(offset + mTargetToWorld.GetTranslation());
}

void Camera::MoveGlobal(float x, float y, float z)
{
	Vector3 offset = Vector3(x, y, z) * Time::DeltaTime();

	mTargetToWorld.SetTranslation(offset + mTargetToWorld.GetTranslation());
}

void CameraEngine::Update(float dt)
{
	mToWorld.SetRotation(Slerp(mToWorld.GetRotation(), mTargetToWorld.GetRotation(), dt * mRotationSmooth));
	mToWorld.SetTranslation(Lerp(mToWorld.GetTranslation(), mTargetToWorld.GetTranslation(), dt * mMoveSmooth));

	mView = Matrix4(~mToWorld);
	mViewProj = mProj * mView;

	mFrustumWS = mToWorld * mFrustumVS;
}