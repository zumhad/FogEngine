#pragma once


namespace Math
{
	class FOG_API Quaternion
	{
	public:
		Quaternion() { mVec = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f); }
		Quaternion(const DirectX::XMMATRIX& m) { mVec = DirectX::XMQuaternionRotationMatrix(m); }
		Quaternion(DirectX::FXMVECTOR v) { mVec = v; }

		static Quaternion Zero() { return DirectX::XMQuaternionIdentity(); }

		operator DirectX::XMVECTOR() const { return mVec; }
		Quaternion& operator= (Quaternion rhs) { mVec = rhs; return *this; }
		Quaternion operator~ (void) const { return Quaternion(DirectX::XMQuaternionConjugate(mVec)); }
		Vector3 operator* (Vector3 rhs) const { return Vector3(DirectX::XMVector3Rotate(rhs, mVec)); }

	private:
		DirectX::XMVECTOR mVec;
	};
}