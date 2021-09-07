#pragma once

using namespace DirectX;

namespace Math
{

	class Scalar
	{
	public:
		Scalar(const Scalar& s) { mVec = s; }
		Scalar(float f) { mVec = XMVectorReplicate(f); }
		Scalar(FXMVECTOR vec) { mVec = vec; }

		operator XMVECTOR() const { return mVec; }
		operator float() const { return XMVectorGetX(mVec); }

	private:
		XMVECTOR mVec;
	};

	inline Scalar operator- (Scalar s) { return Scalar(XMVectorNegate(s)); }
}