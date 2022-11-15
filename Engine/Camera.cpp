#include "Camera.h"
#include "CameraEngine.h"

#include "Input.h"
#include "Time.h"
#include "Trace.h"
#include "Application.h"

using namespace DirectX;
using namespace Math;
using namespace Module;

float Camera::mYaw;
float Camera::mPitch;
float Camera::mRoll;
float Camera::mRotationSmooth;
float Camera::mMoveSmooth;

Vector3 Camera::mPosition;
Vector3 Camera::mRotation;
Vector3 Camera::mTargetPosition;
Vector3 Camera::mTargetRotation;
OrthogonalTransform Camera::mToWorld;
OrthogonalTransform Camera::mTargetToWorld;

Matrix4 Camera::mWorld;
Matrix4 Camera::mView;
Matrix4 Camera::mProj;
Matrix4 Camera::mViewProj;

Frustum Camera::mFrustumVS;
Frustum Camera::mFrustumWS;

float Camera::mFOV;
float Camera::mAspectRatio;
float Camera::mNearZ;
float Camera::mFarZ;
float Camera::mWidth;
float Camera::mHeight;
bool Camera::m3D;

void Camera::LookAt(Vector3 pos)
{
	pos = Normalize(pos - mTargetToWorld.GetTranslation());

	mPitch = ASin(pos.GetY());
	mYaw = ATan2(pos.GetX(), pos.GetZ());
	mRoll = 0;

	mTargetToWorld.SetRotation(XMQuaternionRotationRollPitchYaw(-mPitch, mYaw, mRoll));
}

void CameraEngine::RestartMatrix()
{
	float width, height, aspect;
	if (Singlton.isGame)
	{
		width = (float)Singlton.game.width;
		height = (float)Singlton.game.height;
		aspect = width / height;
	}
	else
	{
		width = (float)Singlton.editor.width;
		height = (float)Singlton.editor.height;
		aspect = (float)Singlton.scene.width / (float)Singlton.scene.height;
	}

	mWidth = width;
	mHeight = height;
	mFOV = Singlton.camera.fov * XM_PI / 180.0f;
	mAspectRatio = aspect;
	mNearZ = Singlton.camera.nearZ;
	mFarZ = Singlton.camera.farZ;
}

void Camera::UpdateProjMatrix()
{
	if (m3D)
	{
		mProj = XMMatrixPerspectiveFovLH(mFOV, mAspectRatio, mNearZ, mFarZ);
		mFrustumVS = Frustum(mProj);
	}
	else
		mProj = XMMatrixOrthographicLH(mWidth, mHeight, mNearZ, mFarZ);
}


void CameraEngine::Setup()
{
	mRotationSmooth = Singlton.camera.rotationSmooth;
	mMoveSmooth = Singlton.camera.moveSmooth;

	mPosition = Vector3::Zero();
	mRotation = Vector3::Zero();
	mTargetPosition = mPosition;
	mTargetRotation = mRotation;

	mWorld = XMMatrixLookAtLH(Vector3(0, 0, -1), Vector3::Zero(), Vector3::OneY());

	RestartMatrix();
	Set3D(true);
}

void Camera::SetRotation(Vector3 rotation)
{
	mRotation = rotation;
	mTargetRotation = mRotation;
}

void Camera::SetRotationX(float x)
{ 
	mRotation.SetX(x);
	mTargetRotation.SetX(x);
}

void Camera::SetRotationY(float y)
{ 
	mRotation.SetY(y);
	mTargetRotation.SetY(y);
}

void Camera::SetRotationZ(float z) 
{ 
	mRotation.SetZ(z);
	mTargetRotation.SetZ(z);
}

void Camera::SetRotation(float x, float y, float z)
{
	mRotation = Vector3(x, y, z);
	mTargetRotation = mRotation;
}

void Camera::Rotate(float x, float y, float z)
{
	mTargetRotation += Vector3(x, y, z);
}

void Camera::SetPosition(Vector3 position)
{
	mPosition = position;
	mTargetPosition = mPosition;
}

void Camera::MoveLocal(float x, float y, float z)
{
	Quaternion q = XMQuaternionRotationRollPitchYawFromVector(mRotation);
	Vector3 offset = Vector3(XMVector3Rotate(Vector3(x, y, z) * Time::DeltaTime(), q));

	mTargetPosition += offset;
}

void Camera::MoveGlobal(float x, float y, float z)
{
	Vector3 offset = Vector3(x, y, z) * Time::DeltaTime();

	mTargetPosition += offset;
}

void CameraEngine::Update(float dt)
{
	mPosition = Lerp(mPosition, mTargetPosition, Pow(0.9f, dt * mMoveSmooth));
	mRotation = Lerp(mRotation, mTargetRotation, Pow(0.9f, dt * mRotationSmooth));

	Quaternion q = XMQuaternionRotationRollPitchYawFromVector(mRotation);
	Vector3 dir = XMVector3Rotate(Vector3::OneZ(), q);
	Vector3 up = XMVector3Rotate(Vector3::OneY(), q);

	mView = XMMatrixLookToLH(mPosition, dir, up);
	mFrustumWS = OrthogonalTransform(q, mPosition) * mFrustumVS;
}