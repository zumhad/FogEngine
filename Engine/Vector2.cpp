#include "Vector2.h"

using namespace DirectX;

Vector2::operator DirectX::XMVECTOR() const
{
    return XMVectorSet(x, y, 0.0f, 1.0f);
}

Vector2::operator DirectX::XMFLOAT2() const
{
    return XMFLOAT2(x, y);
}

Vector2::Vector2(FXMVECTOR V)
{
    XMFLOAT2 f;
    XMStoreFloat2(&f, V);
    x = f.x; y = f.y;
}

Vector2::Vector2(const XMFLOAT2& f)
{
    x = f.x; y = f.y;
}

bool Vector2::operator == (const Vector2& v)
{
    return XMVector2Equal(*this, v);
}

bool Vector2::operator != (const Vector2& v)
{
    return XMVector3NotEqual(*this, v);
}

Vector2& Vector2::operator= (FXMVECTOR v)
{
    XMFLOAT2 f;
    XMStoreFloat2(&f, v);
    x = f.x; y = f.y;

    return *this;
}

Vector2& Vector2::operator= (const XMFLOAT2& f)
{
    x = f.x; y = f.y;

    return *this;
}

Vector2& Vector2::operator+= (const Vector2& v)
{
    *this = XMVectorAdd(*this, v);
    return *this;
}

Vector2& Vector2::operator-= (const Vector2& v)
{
    *this = XMVectorSubtract(*this, v);
    return *this;
}

Vector2& Vector2::operator*= (const Vector2& v)
{
    *this = XMVectorMultiply(*this, v);
    return *this;
}

Vector2& Vector2::operator*= (float f)
{
    *this = XMVectorScale(*this, f);
    return *this;
}

Vector2& Vector2::operator/= (float f)
{
    assert(f != 0.0f);
    *this = XMVectorScale(*this, 1.0f / f);
    return *this;
}

Vector2 Vector2::operator+ ()
{
    return *this;
}

Vector2 Vector2::operator- ()
{
    return XMVectorNegate(*this);
}

Vector2 operator+ (const Vector2& v1, const Vector2& v2)
{
    return XMVectorAdd(v1, v2);
}

Vector2 operator- (const Vector2& v1, const Vector2& v2)
{
    return XMVectorSubtract(v1, v2);
}

Vector2 operator* (const Vector2& v1, const Vector2& v2)
{
    return XMVectorMultiply(v1, v2);
}

Vector2 operator* (const Vector2& v, float f)
{
    return XMVectorScale(v, f);
}

Vector2 operator/ (const Vector2& v1, const Vector2& v2)
{
    return XMVectorDivide(v1, v2);
}

Vector2 operator/ (const Vector2& v, float f)
{
    return XMVectorScale(v, 1.0f / f);
}

Vector2 operator* (float f, const Vector2& v)
{
    return XMVectorScale(v, f);
}