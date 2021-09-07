#pragma once

#include "Vector.h"
using namespace DirectX;

#pragma warning(disable: 4251)

namespace Math
{
	class FOG_API Matrix3
	{
	public:
		Matrix3() {}
		Matrix3(Vector3 x, Vector3 y, Vector3 z) { mMat[0] = x; mMat[1] = y; mMat[2] = z; }
		Matrix3(Quaternion q) { *this = Matrix3(XMMatrixRotationQuaternion(q)); }
		Matrix3(const XMMATRIX& m) { mMat[0] = Vector3(m.r[0]); mMat[1] = Vector3(m.r[1]); mMat[2] = Vector3(m.r[2]); }

		Vector3 GetX() const { return mMat[0]; }
		Vector3 GetY() const { return mMat[1]; }
		Vector3 GetZ() const { return mMat[2]; }

		static Matrix3 MakeXRotation(float angle) { return Matrix3(XMMatrixRotationX(angle)); }
		static Matrix3 MakeYRotation(float angle) { return Matrix3(XMMatrixRotationY(angle)); }
		static Matrix3 MakeZRotation(float angle) { return Matrix3(XMMatrixRotationZ(angle)); }

		Vector3 operator* (Vector3 vec) const { return Vector3(XMVector3TransformNormal(vec, *this)); }
		operator XMMATRIX() const { return (const XMMATRIX&)mMat; }
		Matrix3 operator* (const Matrix3& mat) const { return Matrix3(*this * mat.GetX(), *this * mat.GetY(), *this * mat.GetZ()); }

	private:
		Vector3 mMat[3];
	};

	class FOG_API Matrix4
	{
	public:
		Matrix4() {}
		Matrix4(const XMMATRIX& mat) { mMat = mat; }
		Matrix4(const Matrix3& xyz, Vector3 w)
		{
			mMat.r[0] = XMVectorSetW(xyz.GetX(), 0.0f);
			mMat.r[1] = XMVectorSetW(xyz.GetY(), 0.0f);
			mMat.r[2] = XMVectorSetW(xyz.GetZ(), 0.0f);
			mMat.r[3] = XMVectorSetW(w, 1.0f);
		}

		operator XMMATRIX() const { return mMat; }
		Matrix4 operator= (XMMATRIX m) { return Matrix4(mMat = m); }
		Vector4 operator* (Vector4 vec) const { return Vector4(XMVector4Transform(vec, mMat)); }
		Matrix4 operator* (const Matrix4& mat) const { return Matrix4(XMMatrixMultiply(mMat, mat)); }

	private:
		XMMATRIX mMat = XMMatrixSet(0.0f, 0.0f, 0.0f, 0.0f,
									0.0f, 0.0f, 0.0f, 0.0f,
									0.0f, 0.0f, 0.0f, 0.0f,
									0.0f, 0.0f, 0.0f, 0.0f);
	};

	inline Vector4 Vector4::operator* (Matrix4 mat) const { return Vector4(XMVector4Transform(mVec, mat)); }
}