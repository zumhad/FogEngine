#pragma once

namespace Math
{
	class FOG_API Matrix4
	{
	public:
		Matrix4()
		{
			mMat = DirectX::XMMatrixSet(0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f);
		}

		Matrix4(const OrthogonalTransform& xform) { *this = Matrix4(Matrix3(xform.GetRotation()), xform.GetTranslation()); }
		Matrix4(Vector4 x, Vector4 y, Vector4 z, Vector4 w) { mMat.r[0] = x; mMat.r[1] = y; mMat.r[2] = z; mMat.r[3] = w; }
		Matrix4(const DirectX::XMMATRIX& mat) { mMat = mat; }
		Matrix4(const Matrix3& xyz, Vector3 w)
		{
			mMat.r[0] = DirectX::XMVectorSetW(xyz.GetX(), 0.0f);
			mMat.r[1] = DirectX::XMVectorSetW(xyz.GetY(), 0.0f);
			mMat.r[2] = DirectX::XMVectorSetW(xyz.GetZ(), 0.0f);
			mMat.r[3] = DirectX::XMVectorSetW(w, 1.0f);
		}

		static Matrix4 Identity() { return DirectX::XMMatrixIdentity(); };

		INLINE Vector4 GetX() const { return Vector4(mMat.r[0]); }
		INLINE Vector4 GetY() const { return Vector4(mMat.r[1]); }
		INLINE Vector4 GetZ() const { return Vector4(mMat.r[2]); }
		INLINE Vector4 GetW() const { return Vector4(mMat.r[3]); }

		operator DirectX::XMMATRIX() const { return mMat; }
		Matrix4 operator= (DirectX::XMMATRIX m) { return Matrix4(mMat = m); }
		Vector4 operator* (Vector4 vec) const { return Vector4(DirectX::XMVector4Transform(vec, mMat)); }
		Matrix4 operator* (const Matrix4& mat) const { return Matrix4(DirectX::XMMatrixMultiply(mMat, mat)); }

		operator DirectX::XMFLOAT4X4() const { DirectX::XMFLOAT4X4 res;  DirectX::XMStoreFloat4x4(&res, mMat); return res; }

	private:
		DirectX::XMMATRIX mMat;
	};

	inline Vector4 Vector4::operator* (Matrix4 mat) const { return Vector4(DirectX::XMVector4Transform(mVec, mat)); }
}