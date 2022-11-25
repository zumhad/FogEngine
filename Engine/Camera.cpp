#include "Camera.h"
#include "CameraEngine.h"

#include "Input.h"
#include "Time.h"
#include "Trace.h"
#include "Application.h"

#include <cmath>

using namespace DirectX;
using namespace Module;

float Camera::mRotationSmooth;
float Camera::mMoveSmooth;

XMFLOAT3 Camera::mPosition;
XMFLOAT3 Camera::mRotation;
XMFLOAT3 Camera::mTargetPosition;
XMFLOAT3 Camera::mTargetRotation;

XMFLOAT4X4 Camera::mView;
XMFLOAT4X4 Camera::mProj;

BoundingFrustum Camera::mFrustum;

float Camera::mFOV;
float Camera::mAspectRatio;
float Camera::mNearZ;
float Camera::mFarZ;
float Camera::mWidth;
float Camera::mHeight;
bool Camera::m3D;

void CameraEngine::Set3D(bool var) 
{ 
	m3D = var; 
	Update(Time::DeltaTime()); 
}

void Camera::LookAt(FXMVECTOR pos)
{
	pos;
	return;
}

void CameraEngine::Update()
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


void CameraEngine::Setup()
{
	mRotationSmooth = Singlton.camera.rotationSmooth;
	mMoveSmooth = Singlton.camera.moveSmooth;

	mPosition = XMFLOAT3(0, 0, 0);
	mRotation = XMFLOAT3(0, 0, 0);
	mTargetPosition = mPosition;
	mTargetRotation = mRotation;

	m3D = true;
}

void Camera::SetRotation(FXMVECTOR rotation, bool smooth)
{
	XMStoreFloat3(&mTargetRotation, rotation);
	if(!smooth) mRotation = mTargetRotation;
}

void Camera::SetRotation(float x, float y, float z, bool smooth)
{
	mTargetRotation = XMFLOAT3(x, y, z);
	if (!smooth) mRotation = mTargetRotation;
}

void Camera::SetRotationX(float x, bool smooth)
{ 
	mTargetRotation.x = x;
	if (!smooth) mRotation.x = x;
}

void Camera::SetRotationY(float y, bool smooth)
{ 
	mTargetRotation.y = y;
	if (!smooth) mRotation.y = y;
}

void Camera::SetRotationZ(float z, bool smooth)
{ 
	mTargetRotation.z = z;
	if (!smooth) mRotation.z = z;
}

void Camera::SetPosition(FXMVECTOR position, bool smooth)
{
	XMStoreFloat3(&mTargetPosition, position);
	if (!smooth) mPosition = mTargetPosition;
}

void Camera::SetPosition(float x, float y, float z, bool smooth)
{
	mTargetPosition = XMFLOAT3(x, y, z);
	if (!smooth) mPosition = mTargetPosition;
}

void Camera::SetPositionX(float x, bool smooth)
{
	mTargetPosition.x = x;
	if (!smooth) mPosition.x = x;
}

void Camera::SetPositionY(float y, bool smooth)
{
	mTargetPosition.y = y;
	if (!smooth) mPosition.y = y;
}

void Camera::SetPositionZ(float z, bool smooth)
{
	mTargetPosition.z = z;
	if (!smooth) mPosition.z = z;
}

void Camera::Rotate(float x, float y, float z)
{
	XMVECTOR v = XMVectorSet(x, y, z, 1) + XMLoadFloat3(&mTargetRotation);
	XMStoreFloat3(&mTargetRotation, v);
}

void Camera::MoveLocal(float x, float y, float z)
{
	XMVECTOR q = XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&mRotation));
	XMVECTOR offset = XMVector3Rotate(XMVectorSet(x, y, z, 1) * Time::DeltaTime(), q);
	XMVECTOR v = XMLoadFloat3(&mTargetPosition) + offset;

	XMStoreFloat3(&mTargetPosition, v);
}

void Camera::MoveGlobal(float x, float y, float z)
{
	XMVECTOR offset = XMVectorSet(x, y, z, 1) * Time::DeltaTime();
	XMVECTOR v = XMLoadFloat3(&mTargetPosition) + offset;

	XMStoreFloat3(&mTargetPosition, v);
}

void CameraEngine::Update(float dt)
{
	Update();

	XMVECTOR position = XMLoadFloat3(&mPosition);
	XMVECTOR targetPosition = XMLoadFloat3(&mTargetPosition);
	XMVECTOR rotation = XMLoadFloat3(&mRotation);
	XMVECTOR targetRotation = XMLoadFloat3(&mTargetRotation);

	position = XMVectorLerp(position, targetPosition, std::pow(0.9f, dt * mMoveSmooth));
	rotation = XMVectorLerp(rotation, targetRotation, std::pow(0.9f, dt * mRotationSmooth));
	XMStoreFloat3(&mPosition, position);
	XMStoreFloat3(&mRotation, rotation);

	XMVECTOR q = XMQuaternionRotationRollPitchYawFromVector(rotation);
	XMVECTOR forward = XMVector3Rotate(XMVectorSet(0, 0, 1, 1), q);
	XMVECTOR target = forward + position;
	
	XMMATRIX proj, view;

	if (m3D)
	{
		proj = XMMatrixPerspectiveFovLH(mFOV, mAspectRatio, mNearZ, mFarZ);
		view = XMMatrixLookAtLH(position, target, XMVectorSet(0, 1, 0, 1));
		XMStoreFloat4x4(&mProj, proj);
		XMStoreFloat4x4(&mView, view);

		mFrustum = BoundingFrustum(proj);
		mFrustum.Transform(mFrustum, XMMatrixInverse(0, view));
	}
	else
	{
		proj = XMMatrixOrthographicLH(mWidth, mHeight, mNearZ, mFarZ);
		view = XMMatrixLookAtLH(XMVectorSet(0, 0, -1, 1), XMVectorSet(0, 0, 0, 1), XMVectorSet(0, 1, 0, 1));
		XMStoreFloat4x4(&mProj, proj);
		XMStoreFloat4x4(&mView, view);
	}
}

