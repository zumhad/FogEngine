#include "Camera.h"

#include "MathHelper.h"
#include "Quaternion.h"
#include "CustomArray.h"
#include "Frustum.h"
#include "Application.h"

Vector3 Camera::mOffset;
Vector3 Camera::mPosition;
Vector3 Camera::mRotation;

Matrix Camera::mView;
Matrix Camera::mProj;

float Camera::mFOV;
float Camera::mAspectRatio;
float Camera::mNearZ;
float Camera::mFarZ;

Vector3 Camera::GetPosition()
{ 
	return mOffset + mPosition;
}

Vector3& Camera::GetOffsetPosition()
{
	return mOffset;
}

Vector3& Camera::GetRealPosition()
{
	return mPosition;
}

Vector3 Camera::GetRotation()
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
	Vector3 v = Vector3(0.0f, 0.0f, 1.0f);
	Quaternion q = Quaternion::Euler(Vector3::ConvertToRadians(mRotation));

	return Vector3::Rotate(v, q);
}

void Camera::LookAt(const Vector3& pos)
{
	Vector3 dir = Vector3::Normalize(pos - mOffset - mPosition);

	mRotation.x = -Math::ConvertToDegrees(Math::ASin(dir.y));
	mRotation.y = Math::ConvertToDegrees(Math::ATan2(dir.x, dir.z));
}

void Camera::Setup(const APPCLASS& app)
{
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

	mFOV = Math::ConvertToRadians(app.camera.fov);
	mAspectRatio = aspect;
	mNearZ = app.camera.nearZ;
	mFarZ = app.camera.farZ;
}

void Camera::SetFOV(float fov)
{ 
	mFOV = fov; 
}

void Camera::SetNear(float nearZ)
{
	mNearZ = nearZ;
}

void Camera::SetFar(float farZ)
{ 
	mFarZ = farZ;
}

float Camera::GetFOV()
{
	return mFOV;
}

float Camera::GetNear() 
{ 
	return mNearZ;
}

float Camera::GetFar()
{
	return mFarZ;
}

float Camera::GetAspectRatio() 
{
	return mAspectRatio;
}

void Camera::SetRotation(const Vector3& rotation)
{
	mRotation = rotation;
}

void Camera::SetPosition(const Vector3& position)
{
	mOffset = Vector3((float)(int)position.x, (float)(int)position.y, (float)(int)position.z);
	mPosition = Vector3(position.x - mOffset.x, position.y - mOffset.y, position.z - mOffset.z);

	mOffset.x = Math::Clamp(mOffset.x, -FLOAT_MAX_NUMBER(2), FLOAT_MAX_NUMBER(2));
	mOffset.y = Math::Clamp(mOffset.y, -FLOAT_MAX_NUMBER(2), FLOAT_MAX_NUMBER(2));
	mOffset.z = Math::Clamp(mOffset.z, -FLOAT_MAX_NUMBER(2), FLOAT_MAX_NUMBER(2));

	if (mOffset.x == -FLOAT_MAX_NUMBER(2) || mOffset.x == FLOAT_MAX_NUMBER(2)) mPosition.x = 0.0f;
	if (mOffset.y == -FLOAT_MAX_NUMBER(2) || mOffset.y == FLOAT_MAX_NUMBER(2)) mPosition.y = 0.0f;
	if (mOffset.z == -FLOAT_MAX_NUMBER(2) || mOffset.z == FLOAT_MAX_NUMBER(2)) mPosition.z = 0.0f;
}

void Camera::SetPositionX(float x)
{
	mOffset.x = (float)(int)x;
	mPosition.x = x - mOffset.x;

	mOffset.x = Math::Clamp(mOffset.x, -FLOAT_MAX_NUMBER(2), FLOAT_MAX_NUMBER(2));

	if (mOffset.x == -FLOAT_MAX_NUMBER(2) || mOffset.x == FLOAT_MAX_NUMBER(2)) mPosition.x = 0.0f;
}

void Camera::SetPositionY(float y)
{
	mOffset.y = (float)(int)y;
	mPosition.y = y - mOffset.y;

	mOffset.y = Math::Clamp(mOffset.y, -FLOAT_MAX_NUMBER(2), FLOAT_MAX_NUMBER(2));

	if (mOffset.y == -FLOAT_MAX_NUMBER(2) || mOffset.y == FLOAT_MAX_NUMBER(2)) mPosition.y = 0.0f;
}

void Camera::SetPositionZ(float z)
{
	mOffset.z = (float)(int)z;
	mPosition.z = z - mOffset.z;

	mOffset.z = Math::Clamp(mOffset.z, -FLOAT_MAX_NUMBER(2), FLOAT_MAX_NUMBER(2));

	if (mOffset.z == -FLOAT_MAX_NUMBER(2) || mOffset.z == FLOAT_MAX_NUMBER(2)) mPosition.z = 0.0f;
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

void Camera::Rotate(const Vector3& f)
{
	mRotation += f;
}

void Camera::Move(const Vector3& v)
{
	mPosition += v;

	if (mPosition.x >= 1.0f || mPosition.x <= -1.0f)
	{
		mOffset.x += (int)mPosition.x;
		mPosition.x -= (int)mPosition.x;
	}

	if (mPosition.y >= 1.0f || mPosition.y <= -1.0f)
	{
		mOffset.y += (int)mPosition.y;
		mPosition.y -= (int)mPosition.y;
	}

	if (mPosition.z >= 1.0f || mPosition.z <= -1.0f)
	{
		mOffset.z += (int)mPosition.z;
		mPosition.z -= (int)mPosition.z;
	}

	mOffset.x = Math::Clamp(mOffset.x, -FLOAT_MAX_NUMBER(2), FLOAT_MAX_NUMBER(2));
	mOffset.y = Math::Clamp(mOffset.y, -FLOAT_MAX_NUMBER(2), FLOAT_MAX_NUMBER(2));
	mOffset.z = Math::Clamp(mOffset.z, -FLOAT_MAX_NUMBER(2), FLOAT_MAX_NUMBER(2));

	if (mOffset.x == -FLOAT_MAX_NUMBER(2) || mOffset.x == FLOAT_MAX_NUMBER(2)) mPosition.x = 0.0f;
	if (mOffset.y == -FLOAT_MAX_NUMBER(2) || mOffset.y == FLOAT_MAX_NUMBER(2)) mPosition.y = 0.0f;
	if (mOffset.z == -FLOAT_MAX_NUMBER(2) || mOffset.z == FLOAT_MAX_NUMBER(2)) mPosition.z = 0.0f;
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

	Quaternion q = Quaternion::Euler(Vector3::ConvertToRadians(mRotation));
	Vector3 forward = Vector3::Rotate(Vector3(0.0f, 0.0f, 1.0f), q);
	Vector3 up = Vector3::Rotate(Vector3(0.0f, 1.0f, 0.0f), q);

	mProj = DirectX::XMMatrixPerspectiveFovLH(mFOV, mAspectRatio, mFarZ, mNearZ);
	mView = DirectX::XMMatrixLookAtLH(mPosition, mPosition + forward, up);
}