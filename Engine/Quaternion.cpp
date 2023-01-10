#include "Quaternion.h"

#include "MathHelper.h"

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

Quaternion& Quaternion::operator= (const DirectX::XMFLOAT4& f)
{
    x = f.x; y = f.y; z = f.z; w = f.w;
    return *this;
}

Vector3 Quaternion::GetEulerAngles() const
{
    const float xx = x * x;
    const float yy = y * y;
    const float zz = z * z;

    const float m31 = 2.0f * x * z + 2.0f * y * w;
    const float m32 = 2.0f * y * z - 2.0f * x * w;
    const float m33 = 1.0f - 2.f * xx - 2.0f * yy;

    const float cy = Math::Sqrt(m33 * m33 + m31 * m31);
    const float cx = Math::ATan2(-m32, cy);
    if (cy > 16.0f * Math::Epsilon())
    {
        const float m12 = 2.0f * x * y + 2.0f * z * w;
        const float m22 = 1.0f - 2.0f * xx - 2.0f * zz;

        return Vector3(cx, Math::ATan2(m31, m33), Math::ATan2(m12, m22));
    }
    else
    {
        const float m11 = 1.0f - 2.0f * yy - 2.0f * zz;
        const float m21 = 2.0f * x * y - 2.0f * z * w;

        return Vector3(cx, 0.0f, Math::ATan2(-m21, m11));
    }
}

void Quaternion::SetEulerAngles(float _x, float _y, float _z)
{
    float pitch = Math::ConvertToRadians(_x);
    float yaw = Math::ConvertToRadians(_y);
    float roll = Math::ConvertToRadians(_z);

    *this = XMQuaternionRotationRollPitchYaw(pitch, yaw, roll);
}

Quaternion Quaternion::Euler(float _x, float _y, float _z)
{
    float pitch = Math::ConvertToRadians(_x);
    float yaw = Math::ConvertToRadians(_y);
    float roll = Math::ConvertToRadians(_z);

    return XMQuaternionRotationRollPitchYaw(pitch, yaw, roll);
}

const Quaternion& Quaternion::Identity()
{
    static Quaternion q(0.0f, 0.0f, 0.0f, 1.0f);
    return q;
}

Quaternion Quaternion::FromToRotation(const Vector3& fromDir, const Vector3& toDir)
{
    Quaternion result;

    const XMVECTOR F = XMVector3Normalize(fromDir);
    const XMVECTOR T = XMVector3Normalize(toDir);

    const float dot = XMVectorGetX(XMVector3Dot(F, T));
    if (dot >= 1.0f)
    {
        result = Identity();
    }
    else if (dot <= -1.f)
    {
        XMVECTOR axis = XMVector3Cross(F, Vector3::Right());
        if (XMVector3NearEqual(XMVector3LengthSq(axis), g_XMZero, g_XMEpsilon))
        {
            axis = XMVector3Cross(F, Vector3::Up());
        }

        result = XMQuaternionRotationAxis(axis, XM_PI);
    }
    else
    {
        result = XMVector3Cross(F, T);

        const float s = Math::Sqrt((1.0f + dot) * 2.0f);
        result.x /= s;
        result.y /= s;
        result.z /= s;
        result.w = s * 0.5f;
    }

    return result;
}

Quaternion Quaternion::LookRotation(const Vector3& forward, const Vector3& up)
{
    Quaternion result;

    Quaternion q1 = FromToRotation(Vector3::Forward(), forward);

    const XMVECTOR C = XMVector3Cross(forward, up);
    if (XMVector3NearEqual(XMVector3LengthSq(C), g_XMZero, g_XMEpsilon))
    {
        result = q1;
        return result;
    }

    const XMVECTOR U = XMQuaternionMultiply(q1, Vector3::Up());

    Quaternion q2 = FromToRotation(U, up);
    
    result = XMQuaternionMultiply(q2, q1);
    return result;
}