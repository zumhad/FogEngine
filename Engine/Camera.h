#pragma once

#include "Core.h"
#include "Properties.h"

#include <DirectXMath.h>
#include <DirectXCollision.h>

class FOG_API Camera
{
public:
	static void SetPosition(DirectX::FXMVECTOR position, bool smooth = true);
	static void SetPosition(float x, float y, float z, bool smooth = true);
	static void SetPositionX(float x, bool smooth = true);
	static void SetPositionY(float y, bool smooth = true);
	static void SetPositionZ(float z, bool smooth = true);

	static void SetRotation(DirectX::FXMVECTOR rotation, bool smooth = true);
	static void SetRotation(float x, float y, float z, bool smooth = true);
	static void SetRotationX(float x, bool smooth = true);
	static void SetRotationY(float y, bool smooth = true);
	static void SetRotationZ(float z, bool smooth = true);

	static void LookAt(DirectX::FXMVECTOR pos);
	static void MoveLocal(float x, float y, float z);
	static void MoveGlobal(float x, float y, float z);
	static void Rotate(float x, float y, float z);
	static float GetRotateX() { return mRotation.x; };
	static float GetRotateY() { return mRotation.y; };
	static float GetRotateZ() { return mRotation.z; };

	static DirectX::XMVECTOR GetPosition() { return XMLoadFloat3(&mPosition); }
	static DirectX::XMVECTOR GetRotation() { return XMLoadFloat3(&mRotation); }

protected:
	static float mRotationSmooth;
	static float mMoveSmooth;

	static DirectX::XMFLOAT3 mPosition;
	static DirectX::XMFLOAT3 mRotation;
	static DirectX::XMFLOAT3 mTargetPosition;
	static DirectX::XMFLOAT3 mTargetRotation;

	static DirectX::XMFLOAT4X4 mView;
	static DirectX::XMFLOAT4X4 mProj;

	static DirectX::BoundingFrustum mFrustum;

	static float mWidth;
	static float mHeight;
	static float mFOV;
	static float mAspectRatio;
	static float mNearZ;
	static float mFarZ;
	static bool m3D;
};