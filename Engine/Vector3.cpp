#include "Vector3.h"

#include "MathHelper.h"
#include "Timer.h"
#include "Quaternion.h"

using namespace DirectX;

Vector3::operator DirectX::XMVECTOR() const
{
	return XMVectorSet(x, y, z, 1.0f);
}

Vector3::operator DirectX::XMFLOAT3() const
{
	return XMFLOAT3(x, y, z);
}

Vector3::Vector3(FXMVECTOR V)
{ 
	XMFLOAT3 f;
	XMStoreFloat3(&f, V);
	x = f.x; y = f.y; z = f.z;
}

Vector3::Vector3(const XMFLOAT3& f) 
{ 
	x = f.x; y = f.y; z = f.z;
}

bool Vector3::operator == (const Vector3& v)
{
    return XMVector3Equal(*this, v);
}

bool Vector3::operator != (const Vector3& v)
{
    return XMVector3NotEqual(*this, v);
}

Vector3& Vector3::operator= (FXMVECTOR v)
{
    XMFLOAT3 f;
    XMStoreFloat3(&f, v);
    x = f.x; y = f.y; z = f.z;

    return *this;
}

Vector3& Vector3::operator= (const XMFLOAT3& f)
{
    x = f.x; y = f.y; z = f.z;

    return *this;
}

Vector3& Vector3::operator+= (const Vector3& v)
{
    *this = XMVectorAdd(*this, v);
    return *this;
}

Vector3& Vector3::operator-= (const Vector3& v)
{
    *this = XMVectorSubtract(*this, v);
    return *this;
}

Vector3& Vector3::operator*= (const Vector3& v)
{
    *this = XMVectorMultiply(*this, v);
    return *this;
}

Vector3& Vector3::operator*= (float f)
{
    *this = XMVectorScale(*this, f);
    return *this;
}

Vector3& Vector3::operator/= (float f)
{
    assert(f != 0.0f);
    *this = XMVectorScale(*this, 1.0f / f);
    return *this;
}

Vector3 Vector3::operator+ () const
{
    return *this;
}

Vector3 Vector3::operator- () const
{
    Vector3 v = XMVectorNegate(*this);
    return v;
}



Vector3 operator+ (const Vector3& v1, const Vector3& v2)
{
    Vector3 v = XMVectorAdd(v1, v2);
    return v;
}

Vector3 operator- (const Vector3& v1, const Vector3& v2)
{
    Vector3 v = XMVectorSubtract(v1, v2);
    return v;
}

Vector3 operator* (const Vector3& v1, const Vector3& v2)
{
    Vector3 v = XMVectorMultiply(v1, v2);
    return v;
}

Vector3 operator* (const Vector3& v, float f)
{
    Vector3 _v = XMVectorScale(v, f);
    return _v;
}

Vector3 operator/ (const Vector3& v1, const Vector3& v2)
{
    Vector3 v = XMVectorDivide(v1, v2);
    return v;
}

Vector3 operator/ (const Vector3& v, float f)
{
    Vector3 _v = XMVectorScale(v, 1.0f / f);
    return _v;
}

Vector3 operator* (float f, const Vector3& v)
{
    Vector3 _v = XMVectorScale(v, f);
    return _v;
}


const Vector3& Vector3::Zero()
{
    static Vector3 v(0.0f, 0.0f, 0.0f);
    return v;
}

const Vector3& Vector3::One()
{
    static Vector3 v(1.0f, 1.0f, 1.0f);
    return v;
}

const Vector3& Vector3::Up()
{
    static Vector3 v(0.0f, 1.0f, 0.0f);
    return v;
}

const Vector3& Vector3::Down()
{
    static Vector3 v(0.0f, -1.0f, 0.0f);
    return v;
}

const Vector3& Vector3::Right()
{
    static Vector3 v(1.0f, 0.0f, 0.0f);
    return v;
}

const Vector3& Vector3::Left()
{
    static Vector3 v(-1.0f, 0.0f, 0.0f);
    return v;
}

const Vector3& Vector3::Forward()
{
    static Vector3 v(0.0f, 0.0f, 1.0f);
    return v;
}

const Vector3& Vector3::Backward()
{
    static Vector3 v(0.0f, 0.0f, -1.0f);
    return v;
}

Vector3 Vector3::ConvertToRadians(const Vector3& v)
{
    return v * (XM_PI / 180.0f);
}

Vector3 Vector3::SmoothDamp(const Vector3& current, Vector3 target, Vector3& currentVelocity, float smoothTime)
{
    float maxSpeed = Math::Infinity();
    float deltaTime = Time::DeltaTime();

    float output_x = 0.0f;
    float output_y = 0.0f;
    float output_z = 0.0f;

    smoothTime = Math::Max(0.0001f, smoothTime);
    float omega = 2.0f / smoothTime;

    float x = omega * deltaTime;
    float exp = 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x);

    float change_x = current.x - target.x;
    float change_y = current.y - target.y;
    float change_z = current.z - target.z;
    Vector3 originalTo = target;

    float maxChange = maxSpeed * smoothTime;

    float maxChangeSq = maxChange * maxChange;
    float sqrmag = change_x * change_x + change_y * change_y + change_z * change_z;
    if (sqrmag > maxChangeSq)
    {
        float mag = Math::Sqrt(sqrmag);
        change_x = change_x / mag * maxChange;
        change_y = change_y / mag * maxChange;
        change_z = change_z / mag * maxChange;
    }

    target.x = current.x - change_x;
    target.y = current.y - change_y;
    target.z = current.z - change_z;

    float temp_x = (currentVelocity.x + omega * change_x) * deltaTime;
    float temp_y = (currentVelocity.y + omega * change_y) * deltaTime;
    float temp_z = (currentVelocity.z + omega * change_z) * deltaTime;

    currentVelocity.x = (currentVelocity.x - omega * temp_x) * exp;
    currentVelocity.y = (currentVelocity.y - omega * temp_y) * exp;
    currentVelocity.z = (currentVelocity.z - omega * temp_z) * exp;

    output_x = target.x + (change_x + temp_x) * exp;
    output_y = target.y + (change_y + temp_y) * exp;
    output_z = target.z + (change_z + temp_z) * exp;

    float origMinusCurrent_x = originalTo.x - current.x;
    float origMinusCurrent_y = originalTo.y - current.y;
    float origMinusCurrent_z = originalTo.z - current.z;
    float outMinusOrig_x = output_x - originalTo.x;
    float outMinusOrig_y = output_y - originalTo.y;
    float outMinusOrig_z = output_z - originalTo.z;

    if (origMinusCurrent_x * outMinusOrig_x + origMinusCurrent_y * outMinusOrig_y + origMinusCurrent_z * outMinusOrig_z > 0)
    {
        output_x = originalTo.x;
        output_y = originalTo.y;
        output_z = originalTo.z;

        currentVelocity.x = (output_x - originalTo.x) / deltaTime;
        currentVelocity.y = (output_y - originalTo.y) / deltaTime;
        currentVelocity.z = (output_z - originalTo.z) / deltaTime;
    }

    return Vector3(output_x, output_y, output_z);
}

Vector3 Vector3::ConvertToDegrees(const Vector3& v)
{
    return v * (180.0f / XM_PI);
}

Vector3 Vector3::Normalize(const Vector3& v)
{
    return XMVector3Normalize(v);
}

Vector3 Vector3::Rotate(const Vector3& v, const Quaternion& q)
{
    return XMVector3Rotate(v, q);
}

Vector3 Vector3::Lerp(const Vector3& v1, const Vector3& v2, float t)
{
    return XMVectorLerp(v1, v2, t);
}