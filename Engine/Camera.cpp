#include "CameraEngine.h"

#include "Trace.h"
#include "Application.h"
#include "Properties.h"
#include "MathHelper.h"
#include "FrustumCulling.h"
#include "Quaternion.h"

using namespace DirectX;


Vector3 Camera::mPosition;
Vector3 Camera::mRotation;

Matrix Camera::mView;
Matrix Camera::mProj;

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
	Update(); 
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

void CameraEngine::UpdateProperties()
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
	mPosition = XMFLOAT3(0.0f, 0.0f, 0.0f);
	mRotation = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m3D = true;
}

void Camera::SetRotation(Vector3 rotation)
{
	mRotation = rotation;
}

void Camera::SetRotation(float x, float y, float z)
{
	mRotation = XMFLOAT3(x, y, z);
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
	mPosition = XMFLOAT3(x, y, z);
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

void CameraEngine::Update()
{
	UpdateProperties();

	if (mRotation.x > 90.0f)
		mRotation.x = 90.0f;
	if (mRotation.x < -90.0f)
		mRotation.x = -90.0f;

	XMVECTOR position = mPosition;
	XMVECTOR rotation = Vector3::ConvertToRadians(mRotation);

	XMVECTOR q = XMQuaternionRotationRollPitchYawFromVector(rotation);
	XMVECTOR forward = XMVector3Rotate(XMVectorSet(0, 0, 1, 1), q);
	XMVECTOR target = forward + position;
	
	XMMATRIX proj, view;

	if (m3D)
	{
		proj = XMMatrixPerspectiveFovLH(mFOV, mAspectRatio, mNearZ, mFarZ);
		view = XMMatrixLookAtLH(position, target, XMVectorSet(0, 1, 0, 1));
		mProj = proj;
		mView = view;

		FrustumCulling::Update(view, proj);
	}
	else
	{
		proj = XMMatrixOrthographicLH(mWidth, mHeight, mNearZ, mFarZ);
		view = XMMatrixLookAtLH(XMVectorSet(0, 0, -1, 1), XMVectorSet(0, 0, 0, 1), XMVectorSet(0, 1, 0, 1));
		mProj = proj;
		mView = view;
	}
}

