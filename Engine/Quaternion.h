#pragma once


namespace Math
{
	class FOG_API Quaternion
	{
	public:
		inline Quaternion() { mVec = DirectX::XMQuaternionIdentity(); }
		inline Quaternion(const Vector3& axis, const Scalar& angle) { mVec = DirectX::XMQuaternionRotationAxis(axis, angle); }
		inline Quaternion(float pitch, float yaw, float roll) { mVec = DirectX::XMQuaternionRotationRollPitchYaw(pitch, yaw, roll); }
		inline Quaternion(const DirectX::XMMATRIX& matrix) { mVec = DirectX::XMQuaternionRotationMatrix(matrix); }
		inline Quaternion(DirectX::FXMVECTOR vec) { mVec = vec; }

		static Quaternion Identity() { return DirectX::XMQuaternionIdentity(); }

		Vector3 operator* (Vector3 rhs) const { return Vector3(DirectX::XMVector3Rotate(rhs, mVec)); }
		operator DirectX::XMVECTOR() const { return mVec; }
		Quaternion& operator= (Quaternion rhs) { mVec = rhs; return *this; }
		Quaternion operator~ (void) const { return Quaternion(DirectX::XMQuaternionConjugate(mVec)); }
		Quaternion operator* (Quaternion rhs) const { return Quaternion(DirectX::XMQuaternionMultiply(rhs, mVec)); }

	private:
		DirectX::XMVECTOR mVec;
	};
}