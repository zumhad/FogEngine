#pragma once

#include <algorithm>


namespace Math
{
	inline Scalar ACos(Scalar s) { return Scalar(DirectX::XMVectorACos(s)); }
	inline Scalar ASin(Scalar s) { return Scalar(DirectX::XMVectorASin(s)); }
	inline Scalar Sin(Scalar s) { return Scalar(DirectX::XMVectorSin(s)); }
	inline Scalar ATan2(Scalar y, Scalar x) { return Scalar(DirectX::XMVectorATan2(y, x)); }
	inline Scalar Length(Vector3 v) { return Scalar(DirectX::XMVector3Length(v)); }
	inline Scalar LengthSq(Vector3 v) { return Scalar(DirectX::XMVector3LengthSq(v)); }
	inline Scalar RecipSqrt(Scalar s) { return Scalar(DirectX::XMVectorReciprocalSqrt(s)); }
	inline Scalar Dot(Vector3 v1, Vector3 v2) { return Scalar(DirectX::XMVector3Dot(v1, v2)); }
	inline Scalar Pow(Scalar b, Scalar e) { return Scalar(DirectX::XMVectorPow(b, e)); }

	inline Vector3 Lerp(Vector3 a, Vector3 b, float t)
	{ 
		if (t > 1.0f) return Vector3(DirectX::XMVectorLerp(a, b, 1.0f));
		else return Vector3(DirectX::XMVectorLerp(a, b, t));
	}
	inline Vector3 Cross(Vector3 v1, Vector3 v2) { return Vector3(DirectX::XMVector3Cross(v1, v2)); }
	inline Vector3 Normalize(Vector3 v) { return Vector3(DirectX::XMVector3Normalize(v)); }

	inline Quaternion Normalize(Quaternion q) { return Quaternion(DirectX::XMQuaternionNormalize(q)); }
	inline Quaternion Slerp(Quaternion a, Quaternion b, float t) { return Normalize(Quaternion(DirectX::XMQuaternionSlerp(a, b, t))); }


	//template <typename T> 
	//inline int16 Sign(T val) { return ((val < (0)) ? -1 : 1); }

	inline float ACos(float s) { return ACos(Scalar(s)); }
	inline float ASin(float s) { return ASin(Scalar(s)); }
	inline float Sin(float s) { return Sin(Scalar(s)); }
	inline float Lerp(float a, float b, float t) { return a + (b - a) * t; }
	inline float Pow(float b, float e) { return Pow(Scalar(b), Scalar(e)); }
	inline float Abs(float s) { return s < 0.0f ? -s : s; }
	inline float Max(float a, float b) { return a > b ? a : b; }
	inline float Min(float a, float b) { return a < b ? a : b; }
	inline float Clamp(float v, float a, float b) { return Min(Max(v, a), b); }

	inline Quaternion Lerp(Quaternion a, Quaternion b, float t)
	{
		return Normalize(Quaternion(DirectX::XMVectorLerp(a, b, Min(t, 1.0f))));
	}
}