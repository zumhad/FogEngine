#include "Color.h"

#include <cstring>

using namespace DirectX;

Color::Color(FXMVECTOR V)
{
	XMFLOAT4 f;
	XMStoreFloat4(&f, V);
	r = f.x; g = f.y; b = f.z; a = f.w;
}

Color::Color(const XMFLOAT4& f)
{ 
	r = f.x; g = f.y; b = f.z; a = f.w; 
}

Color::operator DirectX::XMFLOAT4() const
{ 
	return XMFLOAT4(r, g, b, a); 
}

Color::operator DirectX::XMVECTOR() const
{ 
	return XMVectorSet(r, g, b, a);
}

Color::operator const float* () const
{ 
	return reinterpret_cast<const float*>(this); 
}

bool Color::operator== (const Color& c) const
{
    return (r == c.r && g == c.g && b == c.b && a == c.a);
}

bool Color::operator!= (const Color& c) const
{
    return !(*this == c);
}

Color& Color::operator= (FXMVECTOR v)
{
	XMFLOAT4 f;
	XMStoreFloat4(&f, v);
	r = f.x; g = f.y; b = f.z; a = f.w; 

	return *this;
}

Color& Color::operator= (const XMFLOAT4& f)
{
    r = f.x; g = f.y; b = f.z; a = f.w;

    return *this;
}

Color& Color::operator+= (const Color& c)
{
    *this = XMVectorAdd(*this, c);
    return *this;
}

Color& Color::operator-= (const Color& c)
{
    *this = XMVectorSubtract(*this, c);
    return *this;
}

Color& Color::operator*= (const Color& c)
{
    *this = XMVectorMultiply(*this, c);
    return *this;
}

Color& Color::operator*= (float f)
{
    *this = XMVectorScale(*this, f);
    return *this;
}

Color& Color::operator/= (const Color& c)
{
    *this = XMVectorDivide(*this, c);
    return *this;
}

Color Color::operator+ () const
{
    return *this;
}

Color Color::operator- () const
{
    Color c = XMVectorNegate(*this);
    return c;
}

Color operator+ (const Color& c1, const Color& c2)
{
    Color c = XMVectorAdd(c1, c2);
    return c;
}

Color operator- (const Color& c1, const Color& c2)
{
    Color c = XMVectorSubtract(c1, c2);
    return c;
}

Color operator* (const Color& c1, const Color& c2)
{
    Color c = XMVectorMultiply(c1, c2);
    return c;
}

Color operator* (const Color& c, float f)
{
    Color _c = XMVectorScale(c, f);
    return _c;
}

Color operator/ (const Color& c1, const Color& c2)
{
    Color c = XMVectorDivide(c1, c2);
    return c;
}

Color operator* (float f, const Color& c)
{
    Color _c = XMVectorScale(c, f);
    return _c;
}