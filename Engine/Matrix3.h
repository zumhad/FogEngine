#pragma once

#include "Vector.h"

namespace Math
{
	class FOG_API Matrix3
	{
	public:
		Matrix3() {}
		Matrix3(Vector3 x, Vector3 y, Vector3 z) { mMat[0] = x; mMat[1] = y; mMat[2] = z; }
		Matrix3(Quaternion q) { *this = Matrix3(DirectX::XMMatrixRotationQuaternion(q)); }
		Matrix3(const DirectX::XMMATRIX& m) { mMat[0] = Vector3(m.r[0]); mMat[1] = Vector3(m.r[1]); mMat[2] = Vector3(m.r[2]); }

		Vector3 GetX() const { return mMat[0]; }
		Vector3 GetY() const { return mMat[1]; }
		Vector3 GetZ() const { return mMat[2]; }

		static Matrix3 MakeXRotation(float angle) { return Matrix3(DirectX::XMMatrixRotationX(angle)); }
		static Matrix3 MakeYRotation(float angle) { return Matrix3(DirectX::XMMatrixRotationY(angle)); }
		static Matrix3 MakeZRotation(float angle) { return Matrix3(DirectX::XMMatrixRotationZ(angle)); }
		static Matrix3 MakeScale(Vector3 scale) { return Matrix3(DirectX::XMMatrixScalingFromVector(scale)); }

		Vector3 operator* (Vector3 vec) const { return Vector3(DirectX::XMVector3TransformNormal(vec, *this)); }
		operator DirectX::XMMATRIX() const { return (const DirectX::XMMATRIX&)mMat; }
		Matrix3 operator* (const Matrix3& mat) const { return Matrix3(*this * mat.GetX(), *this * mat.GetY(), *this * mat.GetZ()); }

	private:
		Vector3 mMat[3];
	};
}