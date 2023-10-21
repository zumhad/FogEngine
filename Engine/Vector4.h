#pragma once

#include "Core.h"

#include <DirectXMath.h>

class Vector3;

class FOG_API Vector4
{
public:
    Vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
    Vector4(float ix) : x(ix), y(ix), z(ix), w(ix) {}
    Vector4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
    Vector4(const float* arr) : x(arr[0]), y(arr[1]), z(arr[2]), w(arr[3]) {}
    Vector4(DirectX::FXMVECTOR v);
    Vector4(const DirectX::XMFLOAT4& f);

    operator DirectX::XMVECTOR() const;
    operator DirectX::XMFLOAT4() const;
    operator Vector3() const;

    bool operator == (const Vector4& V);
    bool operator != (const Vector4& V);

    Vector4& operator= (DirectX::FXMVECTOR v);
    Vector4& operator= (const DirectX::XMFLOAT4& f);
    Vector4& operator+= (const Vector4& v);
    Vector4& operator-= (const Vector4& v);
    Vector4& operator*= (const Vector4& v);
    Vector4& operator*= (float f);
    Vector4& operator/= (float f);

    Vector4 operator+ ();
    Vector4 operator- ();

public:
	float x, y, z, w;
};

FOG_API Vector4 operator+ (const Vector4& v1, const Vector4& v2);
FOG_API Vector4 operator- (const Vector4& v1, const Vector4& v2);
FOG_API Vector4 operator* (const Vector4& v1, const Vector4& v2);
FOG_API Vector4 operator* (const Vector4& v, float f);
FOG_API Vector4 operator/ (const Vector4& v1, const Vector4& v2);
FOG_API Vector4 operator/ (const Vector4& v, float f);
FOG_API Vector4 operator* (float f, const Vector4& v);
