#pragma once


namespace Math
{

	class Scalar
	{
	public:
		Scalar(const Scalar& s) { mVec = s; }
		Scalar(float f) { mVec = DirectX::XMVectorReplicate(f); }
		Scalar(DirectX::FXMVECTOR vec) { mVec = vec; }

		operator DirectX::XMVECTOR() const { return mVec; }
		operator float() const { return DirectX::XMVectorGetX(mVec); }

	private:
		DirectX::XMVECTOR mVec;
	};

	inline Scalar operator- (Scalar s) { return Scalar(DirectX::XMVectorNegate(s)); }
}