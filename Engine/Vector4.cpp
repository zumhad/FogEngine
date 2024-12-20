#include "Vector4.h"

#include "Vector3.h"
#include "Utility.h"
#include "Matrix.h"

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

Vector4::Vector4(Vector3 v, float f)
{
	x = v.x; y = v.y; z = v.z; w = f;
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
	*this = XMVECTOR(*this) + XMVECTOR(v);
	return *this;
}

Vector4& Vector4::operator-= (const Vector4& v)
{
	*this = XMVECTOR(*this) - XMVECTOR(v);
	return *this;
}

Vector4& Vector4::operator*= (const Vector4& v)
{
	*this = XMVECTOR(*this) * XMVECTOR(v);
	return *this;
}

Vector4& Vector4::operator*= (float f)
{
	*this = XMVECTOR(*this) * f;
	return *this;
}

Vector4& Vector4::operator/= (float f)
{
	FOG_ASSERT(f != 0.0f);

	*this = XMVECTOR(*this) / f;

	return *this;
}

Vector4 Vector4::operator+ ()
{
	return +XMVECTOR(*this);
}

Vector4 Vector4::operator- ()
{
	return -XMVECTOR(*this);
}

Vector4 operator+ (const Vector4& v1, const Vector4& v2)
{
	return XMVECTOR(v1) + XMVECTOR(v2);
}

Vector4 operator- (const Vector4& v1, const Vector4& v2)
{
	return XMVECTOR(v1) - XMVECTOR(v2);
}

Vector4 operator* (const Vector4& v1, const Vector4& v2)
{
	return XMVECTOR(v1) * XMVECTOR(v2);
}

Vector4 operator* (const Vector4& v, float f)
{
	return XMVECTOR(v) * f;
}

Vector4 operator/ (const Vector4& v1, const Vector4& v2)
{
	return XMVECTOR(v1) / XMVECTOR(v2);
}

Vector4 operator/ (const Vector4& v, float f)
{
	return XMVECTOR(v) / f;
}

Vector4 operator* (float f, const Vector4& v)
{
	return XMVECTOR(v) * f;
}

Vector4 Vector4::Normalize(const Vector4& v)
{
	return XMVector4Normalize(v);
}

Vector4 Vector4::Transform(const Vector4& v, const Matrix& m)
{
	return XMVector4Transform(v, m);
}

Vector4 Vector4::Reciprocal(const Vector4& v)
{
	return XMVectorReciprocal(v);
}