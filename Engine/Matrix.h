#pragma once

#include "Vector.h"

#pragma warning(disable: 4251)

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

		Vector3 operator* (Vector3 vec) const { return Vector3(DirectX::XMVector3TransformNormal(vec, *this)); }
		operator DirectX::XMMATRIX() const { return (const DirectX::XMMATRIX&)mMat; }
		Matrix3 operator* (const Matrix3& mat) const { return Matrix3(*this * mat.GetX(), *this * mat.GetY(), *this * mat.GetZ()); }

	private:
		Vector3 mMat[3];
	};

	class FOG_API Matrix4
	{
	public:
		Matrix4() {}
		Matrix4(const DirectX::XMMATRIX& mat) { mMat = mat; }
		Matrix4(const Matrix3& xyz, Vector3 w)
		{
			mMat.r[0] = DirectX::XMVectorSetW(xyz.GetX(), 0.0f);
			mMat.r[1] = DirectX::XMVectorSetW(xyz.GetY(), 0.0f);
			mMat.r[2] = DirectX::XMVectorSetW(xyz.GetZ(), 0.0f);
			mMat.r[3] = DirectX::XMVectorSetW(w, 1.0f);
		}

		operator DirectX::XMMATRIX() const { return mMat; }
		Matrix4 operator= (DirectX::XMMATRIX m) { return Matrix4(mMat = m); }
		Vector4 operator* (Vector4 vec) const { return Vector4(DirectX::XMVector4Transform(vec, mMat)); }
		Matrix4 operator* (const Matrix4& mat) const { return Matrix4(DirectX::XMMatrixMultiply(mMat, mat)); }

	private:
		DirectX::XMMATRIX mMat = DirectX::XMMatrixSet(0.0f, 0.0f, 0.0f, 0.0f,
									0.0f, 0.0f, 0.0f, 0.0f,
									0.0f, 0.0f, 0.0f, 0.0f,
									0.0f, 0.0f, 0.0f, 0.0f);
	};

	inline Vector4 Vector4::operator* (Matrix4 mat) const { return Vector4(DirectX::XMVector4Transform(mVec, mat)); }
}