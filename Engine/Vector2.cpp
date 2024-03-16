#include "Vector2.h"

#include "Utility.h"

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
    return XMVector2NotEqual(*this, v);
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
    *this = XMVECTOR(*this) + XMVECTOR(v);
    return *this;
}

Vector2& Vector2::operator-= (const Vector2& v)
{
    *this = XMVECTOR(*this) - XMVECTOR(v);
    return *this;
}

Vector2& Vector2::operator*= (const Vector2& v)
{
    *this = XMVECTOR(*this) + XMVECTOR(v);
    return *this;
}

Vector2& Vector2::operator*= (float f)
{
    *this = XMVECTOR(*this) * f;
    return *this;
}

Vector2& Vector2::operator/= (float f)
{
    FOG_ASSERT(f != 0.0f);

    *this = XMVECTOR(*this) / f;

    return *this;
}

Vector2 Vector2::operator+ ()
{
    return +XMVECTOR(*this);
}

Vector2 Vector2::operator- ()
{
    return -XMVECTOR(*this);
}

Vector2 operator+ (const Vector2& v1, const Vector2& v2)
{
    return XMVECTOR(v1) + XMVECTOR(v2);
}

Vector2 operator- (const Vector2& v1, const Vector2& v2)
{
    return XMVECTOR(v1) - XMVECTOR(v2);
}

Vector2 operator* (const Vector2& v1, const Vector2& v2)
{
    return XMVECTOR(v1) * XMVECTOR(v2);
}

Vector2 operator* (const Vector2& v, float f)
{
    return XMVECTOR(v) * f;
}

Vector2 operator/ (const Vector2& v1, const Vector2& v2)
{
    return XMVECTOR(v1) / XMVECTOR(v2);
}

Vector2 operator/ (const Vector2& v, float f)
{
    return XMVECTOR(v) / f;
}

Vector2 operator* (float f, const Vector2& v)
{
    return XMVECTOR(v) * f;
}