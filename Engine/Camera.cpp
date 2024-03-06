#include "Camera.h"

#include "MathHelper.h"
#include "Quaternion.h"
#include "CustomArray.h"
#include "Frustum.h"
#include "Application.h"

using namespace DirectX;

Vector3 Camera::mPosition;
Vector3 Camera::mRotation;

Matrix Camera::mView;
Matrix Camera::mProj;

float Camera::mFOV;
float Camera::mAspectRatio;
float Camera::mNearZ;
float Camera::mFarZ;

float Camera::GetRotateX() 
{
	return mRotation.x; 
}

float Camera::GetRotateY() 
{
	return mRotation.y;
}

float Camera::GetRotateZ()
{ 
	return mRotation.z;
}

Vector3& Camera::GetPosition()
{ 
	return mPosition;
}

Vector3& Camera::GetRotation()
{
	return mRotation; 
}

Matrix& Camera::GetViewMatrix()
{
	return mView;
}
Matrix& Camera::GetProjMatrix()
{ 
	return mProj; 
}

Vector3 Camera::GetDirection()
{
	XMVECTOR v = XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f);
	XMVECTOR q = XMQuaternionRotationRollPitchYawFromVector(Vector3::ConvertToRadians(mRotation));

	return XMVector3Rotate(v, q);
}

void Camera::LookAt(Vector3 pos)
{
	Vector3 dir = Vector3::Normalize(pos - mPosition);

	mRotation.x = -Math::ConvertToDegrees(Math::ASin(dir.y));
	mRotation.y = Math::ConvertToDegrees(Math::ATan2(dir.x, dir.z));
}

void Camera::Setup(APPCLASS& app)
{
	mPosition = Vector3(0.0f, 0.0f, 0.0f);
	mRotation = Vector3(0.0f, 0.0f, 0.0f);

	float width, height, aspect;

	if (Application::IsGame())
	{
		width = (float)app.game.width;
		height = (float)app.game.height;
		aspect = width / height;
	}
	else
	{
		width = (float)app.scene.width;
		height = (float)app.scene.height;
		aspect = width / height;
	}

	mFOV = app.camera.fov * XM_PI / 180.0f;
	mAspectRatio = aspect;
	mNearZ = app.camera.nearZ;
	mFarZ = app.camera.farZ;
}

void Camera::SetFOV(float fov) { mFOV = fov; }

void Camera::SetNear(float nearZ) { mNearZ = nearZ; }

void Camera::SetFar(float farZ) { mFarZ = farZ; }

float Camera::GetFOV() { return mFOV; }

float Camera::GetNear() { return mNearZ; }

float Camera::GetFar() { return mFarZ; }

float Camera::GetAspectRatio() { return mAspectRatio; }

void Camera::SetRotation(Vector3 rotation)
{
	mRotation = rotation;
}

void Camera::SetRotation(float x, float y, float z)
{
	mRotation = Vector3(x, y, z);
}

void Camera::SetRotationX(float x)
{ 
	mRotation.x = x;
}

void Camera::SetRotationY(float y)
{ 
	mRotation.y = y;
}

void Camera::SetRotationZ(float z)
{ 
	mRotation.z = z;
}

void Camera::SetPosition(Vector3 position)
{
	mPosition = position;
}

void Camera::SetPosition(float x, float y, float z)
{
	mPosition = Vector3(x, y, z);
}

void Camera::SetPositionX(float x)
{
	mPosition.x = x;
}

void Camera::SetPositionY(float y)
{
	mPosition.y = y;
}

void Camera::SetPositionZ(float z)
{
	mPosition.z = z;
}

void Camera::Rotate(Vector3 f)
{
	mRotation += f;
}

void Camera::Move(Vector3 f)
{
	mPosition += f;
}

void Camera::Update()
{
	if (Application::IsGame())
	{
		float width = (float)Application::GetGameWidth();
		float height = (float)Application::GetGameHeight();
		mAspectRatio = width / height;
	}
	else
	{
		float width = (float)Application::GetSceneWidth();
		float height = (float)Application::GetSceneHeight();
		mAspectRatio = width / height;
	}

	if (mRotation.x > 90.0f)
		mRotation.x = 90.0f;
	if (mRotation.x < -90.0f)
		mRotation.x = -90.0f;

	XMVECTOR position = mPosition;
	XMVECTOR rotation = Vector3::ConvertToRadians(mRotation);

	XMVECTOR q = XMQuaternionRotationRollPitchYawFromVector(rotation);
	XMVECTOR forward = XMVector3Rotate(XMVectorSet(0, 0, 1, 1), q);
	XMVECTOR up = XMVector3Rotate(XMVectorSet(0, 1, 0, 1), q);

	XMVECTOR target = forward + position;
	
	mProj = XMMatrixPerspectiveFovLH(mFOV, mAspectRatio, mFarZ, mNearZ);
	mView = XMMatrixLookAtLH(position, target, up);
}