#pragma once


namespace Math
{

	class Scalar
	{
	public:
		Scalar() { mVec = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f); }
		Scalar(const Scalar& s) { mVec = s; }
		Scalar(float f) { mVec = DirectX::XMVectorReplicate(f); }
		Scalar(DirectX::FXMVECTOR vec) { mVec = vec; }

		operator DirectX::XMVECTOR() const { return mVec; }
		operator float() const { return DirectX::XMVectorGetX(mVec); }

	private:
		DirectX::XMVECTOR mVec;
	};

    inline Scalar operator- (Scalar s) { return Scalar(DirectX::XMVectorNegate(s)); }
    inline Scalar operator+ (Scalar s1, Scalar s2) { return Scalar(DirectX::XMVectorAdd(s1, s2)); }
    inline Scalar operator- (Scalar s1, Scalar s2) { return Scalar(DirectX::XMVectorSubtract(s1, s2)); }
    inline Scalar operator* (Scalar s1, Scalar s2) { return Scalar(DirectX::XMVectorMultiply(s1, s2)); }
    inline Scalar operator/ (Scalar s1, Scalar s2) { return Scalar(DirectX::XMVectorDivide(s1, s2)); }
    inline Scalar operator+ (Scalar s1, float s2) { return s1 + Scalar(s2); }
    inline Scalar operator- (Scalar s1, float s2) { return s1 - Scalar(s2); }
    inline Scalar operator* (Scalar s1, float s2) { return s1 * Scalar(s2); }
    inline Scalar operator/ (Scalar s1, float s2) { return s1 / Scalar(s2); }
    inline Scalar operator+ (float s1, Scalar s2) { return Scalar(s1) + s2; }
    inline Scalar operator- (float s1, Scalar s2) { return Scalar(s1) - s2; }
    inline Scalar operator* (float s1, Scalar s2) { return Scalar(s1) * s2; }
    inline Scalar operator/ (float s1, Scalar s2) { return Scalar(s1) / s2; }
}