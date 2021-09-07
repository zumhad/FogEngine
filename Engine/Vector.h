#pragma once

using namespace DirectX;

namespace Math
{
	class FOG_API Vector3
	{
	public:
		Vector3() { mVec = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f); }
		Vector3(float x, float y, float z) { mVec = XMVectorSet(x, y, z, z); }
		Vector3(const XMFLOAT3& v) { mVec = XMLoadFloat3(&v); }
		Vector3(const Vector3& v) { mVec = v; }
		Vector3(Scalar s) { mVec = s; }

		Scalar GetX() const { return Scalar(XMVectorSplatX(mVec));; }
		Scalar GetY() const { return Scalar(XMVectorSplatY(mVec));; }
		Scalar GetZ() const { return Scalar(XMVectorSplatZ(mVec));; }
		void SetX(Scalar x) { mVec = XMVectorPermute<4, 1, 2, 3>(mVec, x); }
		void SetY(Scalar y) { mVec = XMVectorPermute<0, 5, 2, 3>(mVec, y); }
		void SetZ(Scalar z) { mVec = XMVectorPermute<0, 1, 6, 3>(mVec, z); }

		static Vector3 Up() { return Vector3(0.0f, 1.0f, 0.0f); }
		static Vector3 Zero() { return Vector3(0.0f, 0.0f, 0.0f); }
		
		Vector3 operator* (Vector3 v2) const { return Vector3(XMVectorMultiply(mVec, v2)); }
		Vector3 operator* (Scalar  v2) const { return *this * Vector3(v2); }
		Vector3 operator+ (Vector3 v) const { return Vector3(XMVectorAdd(mVec, v)); }
		Vector3 operator- () const { return Vector3(XMVectorNegate(mVec)); }
		Vector3 operator- (Vector3 v) const { return Vector3(XMVectorSubtract(mVec, v)); }
		Vector3 operator* (float  v2) const { return *this * Scalar(v2); }

		Vector3& operator+= (Vector3 v) { *this = *this + v; return *this; }

		operator XMVECTOR() const { return mVec; }

	private:
		XMVECTOR mVec;
	};

	class Matrix4;

	class FOG_API Vector4
	{
	public:
		Vector4() {}
		Vector4(FXMVECTOR v) { mVec = v; }
		Vector4(float x, float y, float z, float w) { mVec = XMVectorSet(x, y, z, w); }

		Scalar GetX() const { return Scalar(XMVectorSplatX(mVec)); }
		Scalar GetY() const { return Scalar(XMVectorSplatY(mVec)); }
		Scalar GetZ() const { return Scalar(XMVectorSplatZ(mVec)); }
		Scalar GetW() const { return Scalar(XMVectorSplatW(mVec)); }

		operator XMVECTOR() const { return mVec; }
		Vector4 operator* (Matrix4 mat) const;

	private:
		XMVECTOR mVec = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	};
}