#include "Vector4.h"

#include "Vector3.h"

using namespace DirectX;

Vector4::Vector4(FXMVECTOR v)
{
	XMFLOAT4 f;
	XMStoreFloat4(&f, v);
	x = f.x; y = f.y; z = f.z, w = f.w;
}

Vector4::Vector4(const XMFLOAT4& f)
{
	x = f.x; y = f.y; z = f.z; w = f.w;
}

Vector4::operator DirectX::XMVECTOR() const
{
	return XMVectorSet(x, y, z, w);
}

Vector4::operator DirectX::XMFLOAT4() const
{
	return XMFLOAT4(x, y, z, w);
}

Vector4::operator Vector3() const
{
	return Vector3(x, y, z);
}

bool Vector4::operator == (const Vector4& v)
{
	return XMVector4Equal(*this, v);
}

bool Vector4::operator != (const Vector4& v)
{
	return XMVector4NotEqual(*this, v);
}

Vector4& Vector4::operator= (FXMVECTOR v)
{
	XMFLOAT4 f;
	XMStoreFloat4(&f, v);
	x = f.x; y = f.y; z = f.z; w = f.w;

	return *this;
}

Vector4& Vector4::operator= (const XMFLOAT4& f)
{
	x = f.x; y = f.y; z = f.z; w = f.w;

	return *this;
}

Vector4& Vector4::operator+= (const Vector4& v)
{
	*this = XMVectorAdd(*this, v);
	return *this;
}

Vector4& Vector4::operator-= (const Vector4& v)
{
	*this = XMVectorSubtract(*this, v);
	return *this;
}

Vector4& Vector4::operator*= (const Vector4& v)
{
	*this = XMVectorMultiply(*this, v);
	return *this;
}

Vector4& Vector4::operator*= (float f)
{
	*this = XMVectorScale(*this, f);
	return *this;
}

Vector4& Vector4::operator/= (float f)
{
	assert(f != 0.0f);
	*this = XMVectorScale(*this, 1.0f / f);
	return *this;
}

Vector4 Vector4::operator+ ()
{
	return *this;
}

Vector4 Vector4::operator- ()
{
	return XMVectorNegate(*this);
}

Vector4 operator+ (const Vector4& v1, const Vector4& v2)
{
	return XMVectorAdd(v1, v2);;
}

Vector4 operator- (const Vector4& v1, const Vector4& v2)
{
	return XMVectorSubtract(v1, v2);
}

Vector4 operator* (const Vector4& v1, const Vector4& v2)
{
	return XMVectorMultiply(v1, v2);
}

Vector4 operator* (const Vector4& v, float f)
{
	return XMVectorScale(v, f);
}

Vector4 operator/ (const Vector4& v1, const Vector4& v2)
{
	return XMVectorDivide(v1, v2);
}

Vector4 operator/ (const Vector4& v, float f)
{
	return XMVectorScale(v, 1.0f / f);
}

Vector4 operator* (float f, const Vector4& v)
{
	return XMVectorScale(v, f);
}