#include "Quaternion.h"

#include "MathHelper.h"
#include "Matrix.h"

using namespace DirectX;

Quaternion::operator DirectX::XMVECTOR() const
{
    return XMVectorSet(x, y, z, w);
}

Quaternion::operator DirectX::XMFLOAT4() const
{
    return XMFLOAT4(x, y, z, w);
}

Quaternion::Quaternion(DirectX::FXMVECTOR v)
{
    XMFLOAT4 f;
    XMStoreFloat4(&f, v);
    x = f.x; y = f.y; z = f.z; w = f.w;
}

Quaternion& Quaternion::operator= (DirectX::FXMVECTOR v)
{
    XMFLOAT4 f;
    XMStoreFloat4(&f, v);
    x = f.x; y = f.y; z = f.z; w = f.w;

    return *this;
}

bool Quaternion::operator == (const Quaternion& q) const
{
    return XMQuaternionEqual(*this, q);
}

bool Quaternion::operator != (const Quaternion& q) const
{
    return XMQuaternionNotEqual(*this, q);
}

Quaternion& Quaternion::operator+= (const Quaternion& q)
{
    *this = XMVectorAdd(*this, q);
    return *this;
}

Quaternion& Quaternion::operator-= (const Quaternion& q)
{
    *this = XMVectorSubtract(*this, q);
    return *this;
}

Quaternion& Quaternion::operator*= (const Quaternion& q)
{
    *this = XMQuaternionMultiply(*this, q);
    return *this;
}

Quaternion& Quaternion::operator*= (float f)
{
    *this = XMVectorScale(*this, f);
    return *this;
}

Quaternion& Quaternion::operator/= (const Quaternion& q)
{
    *this = XMQuaternionMultiply(*this, XMQuaternionInverse(q));
    return *this;
}

Quaternion Quaternion::operator+ () const
{
    return *this;
}

Quaternion Quaternion::operator- () const
{
    return XMVectorNegate(*this);
}

Quaternion operator+ (const Quaternion& q1, const Quaternion& q2)
{
    return XMVectorAdd(q1, q2);
}

Quaternion operator- (const Quaternion& q1, const Quaternion& q2)
{
    return XMVectorSubtract(q1, q2);
}

Quaternion operator* (const Quaternion& q1, const Quaternion& q2)
{
    return XMQuaternionMultiply(q1, q2);
}

Quaternion operator* (const Quaternion& q, float f)
{
    return XMVectorScale(q, f);
}

Quaternion operator/ (const Quaternion& q1, const Quaternion& q2)
{
    return XMQuaternionMultiply(q1, XMQuaternionInverse(q2));
}

Quaternion operator* (float f, const Quaternion& q)
{
    return XMVectorScale(q, f);
}

Quaternion& Quaternion::operator= (const DirectX::XMFLOAT4& f)
{
    x = f.x; y = f.y; z = f.z; w = f.w;
    return *this;
}

Vector3 Quaternion::GetEuler(const Quaternion& q)
{
    const float xx = q.x * q.x;
    const float yy = q.y * q.y;
    const float zz = q.z * q.z;

    const float m31 = 2.0f * q.x * q.z + 2.0f * q.y * q.w;
    const float m32 = 2.0f * q.y * q.z - 2.0f * q.x * q.w;
    const float m33 = 1.0f - 2.f * xx - 2.0f * yy;

    const float cy = Math::Sqrt(m33 * m33 + m31 * m31);
    const float cx = Math::ATan2(-m32, cy);
    if (cy > 16.0f * Math::Epsilon())
    {
        const float m12 = 2.0f * q.x * q.y + 2.0f * q.z * q.w;
        const float m22 = 1.0f - 2.0f * xx - 2.0f * zz;

        return Vector3(cx, Math::ATan2(m31, m33), Math::ATan2(m12, m22));
    }
    else
    {
        const float m11 = 1.0f - 2.0f * yy - 2.0f * zz;
        const float m21 = 2.0f * q.x * q.y - 2.0f * q.z * q.w;

        return Vector3(cx, 0.0f, Math::ATan2(-m21, m11));
    }
}

Quaternion Quaternion::Rotation(const Matrix& m)
{
    return XMQuaternionRotationMatrix(m);
}

Quaternion Quaternion::Euler(const Vector3& v)
{
    return XMQuaternionRotationRollPitchYawFromVector(v);
}

Quaternion Quaternion::Euler(float _x, float _y, float _z)
{
    return XMQuaternionRotationRollPitchYaw(_x, _y, _z);
}

const Quaternion& Quaternion::Identity()
{
    static Quaternion q(0.0f, 0.0f, 0.0f, 1.0f);
    return q;
}