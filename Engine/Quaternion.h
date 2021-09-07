#pragma once

using namespace DirectX;

namespace Math
{
	class FOG_API Quaternion
	{
	public:
		Quaternion() { mVec = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f); }
		Quaternion(const XMMATRIX& m) { mVec = XMQuaternionRotationMatrix(m); }
		Quaternion(FXMVECTOR v) { mVec = v; }

		static Quaternion Zero() { return XMQuaternionIdentity(); }

		operator XMVECTOR() const { return mVec; }
		Quaternion& operator= (Quaternion rhs) { mVec = rhs; return *this; }
		Quaternion operator~ (void) const { return Quaternion(XMQuaternionConjugate(mVec)); }
		Vector3 operator* (Vector3 rhs) const { return Vector3(XMVector3Rotate(rhs, mVec)); }

	private:
		XMVECTOR mVec;
	};
}