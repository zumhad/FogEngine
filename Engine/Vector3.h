#pragma once

#include "Core.h"

#include <DirectXMath.h>

class Quaternion;

class FOG_API Vector3
{
public:
	Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
	Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
	Vector3(float ix) : x(ix), y(ix), z(ix) {}
    Vector3(const float* arr) : x(arr[0]), y(arr[1]), z(arr[2]) {}
	Vector3(DirectX::FXMVECTOR V);
	Vector3(const DirectX::XMFLOAT3& f);

	operator DirectX::XMVECTOR() const;
	operator DirectX::XMFLOAT3() const;

    bool operator == (const Vector3& v);
    bool operator != (const Vector3& v);

    Vector3& operator= (DirectX::FXMVECTOR v);
    Vector3& operator= (const DirectX::XMFLOAT3& f);
    Vector3& operator+= (const Vector3& v);
    Vector3& operator-= (const Vector3& v);
    Vector3& operator*= (const Vector3& v);
    Vector3& operator*= (float f);
    Vector3& operator/= (float f);

    Vector3 operator+ ();
    Vector3 operator- ();

    static const Vector3& Zero();
    static const Vector3& One();
    static const Vector3& Up();
    static const Vector3& Down();
    static const Vector3& Right();
    static const Vector3& Left();
    static const Vector3& Forward();
    static const Vector3& Backward();

    static Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);
    static Vector3 Rotate(const Vector3& v, const Quaternion& q);
    static Vector3 Normalize(const Vector3& v);
    static Vector3 ConvertToDegrees(const Vector3& v);
    static Vector3 ConvertToRadians(const Vector3& v);
    static Vector3 SmoothDamp(const Vector3& current, Vector3 target, Vector3& currentVelocity, float smoothTime);

public:
	float x, y, z;
};

FOG_API Vector3 operator+ (const Vector3& v1, const Vector3& v2);
FOG_API Vector3 operator- (const Vector3& v1, const Vector3& v2);
FOG_API Vector3 operator* (const Vector3& v1, const Vector3& v2);
FOG_API Vector3 operator* (const Vector3& v, float f);
FOG_API Vector3 operator/ (const Vector3& v1, const Vector3& v2);
FOG_API Vector3 operator/ (const Vector3& v, float f);
FOG_API Vector3 operator* (float f, const Vector3& v);
