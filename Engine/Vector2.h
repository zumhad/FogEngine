#pragma once

#include "Core.h"

#include <DirectXMath.h>

class FOG_API Vector2
{
public:
    Vector2() : x(0.0f), y(0.0f) {}
    Vector2(float _x, float _y) : x(_x), y(_y) {}
    Vector2(float ix) : x(ix), y(ix) {}
    Vector2(const float* arr) : x(arr[0]), y(arr[1]) {}
    Vector2(DirectX::FXMVECTOR V);
    Vector2(const DirectX::XMFLOAT2& f);

    operator DirectX::XMVECTOR() const;
    operator DirectX::XMFLOAT2() const;

    bool operator == (const Vector2& v);
    bool operator != (const Vector2& v);

    Vector2& operator= (DirectX::FXMVECTOR v);
    Vector2& operator= (const DirectX::XMFLOAT2& f);
    Vector2& operator+= (const Vector2& v);
    Vector2& operator-= (const Vector2& v);
    Vector2& operator*= (const Vector2& v);
    Vector2& operator*= (float f);
    Vector2& operator/= (float f);

    Vector2 operator+ ();
    Vector2 operator- ();

public:
    float x, y;
};

FOG_API Vector2 operator+ (const Vector2& v1, const Vector2& v2);
FOG_API Vector2 operator- (const Vector2& v1, const Vector2& v2);
FOG_API Vector2 operator* (const Vector2& v1, const Vector2& v2);
FOG_API Vector2 operator* (const Vector2& v, float f);
FOG_API Vector2 operator/ (const Vector2& v1, const Vector2& v2);
FOG_API Vector2 operator/ (const Vector2& v, float f);
FOG_API Vector2 operator* (float f, const Vector2& v);
