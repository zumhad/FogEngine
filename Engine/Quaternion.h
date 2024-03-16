#pragma once

#include "Core.h"

#include "Vector3.h"

#include <DirectXMath.h>

class FOG_API Vector3;

class FOG_API Quaternion
{
public:
    Quaternion() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
    Quaternion(float ix, float iy, float iz, float iw) : x(ix), y(iy), z(iz), w(iw) {}
    Quaternion(const Vector3& v, float scalar) : x(v.x), y(v.y), z(v.z), w(scalar) {}
    Quaternion(const DirectX::XMFLOAT4& f) : x(f.x), y(f.y), z(f.z), w(f.w) {}
    Quaternion(DirectX::FXMVECTOR v);

    operator DirectX::XMVECTOR() const;
    operator DirectX::XMFLOAT4() const;

    bool operator == (const Quaternion& q) const;
    bool operator != (const Quaternion& q) const;

    Quaternion& operator= (DirectX::FXMVECTOR v);
    Quaternion& operator= (const DirectX::XMFLOAT4& f);
    Quaternion& operator+= (const Quaternion& q);
    Quaternion& operator-= (const Quaternion& q);
    Quaternion& operator*= (const Quaternion& q);
    Quaternion& operator*= (float S);
    Quaternion& operator/= (const Quaternion& q);

    Quaternion operator+ () const;
    Quaternion operator- () const;

    static Quaternion Rotation(const Matrix& m);
    static Vector3 GetEuler(const Quaternion& q);
    static Quaternion Euler(const Vector3& v);
    static Quaternion Euler(float x, float y, float z);

    static const Quaternion& Identity();

public:
	float x, y, z, w;
};

FOG_API Quaternion operator+ (const Quaternion& q1, const Quaternion& q2);
FOG_API Quaternion operator- (const Quaternion& q1, const Quaternion& q2);
FOG_API Quaternion operator* (const Quaternion& q1, const Quaternion& q2);
FOG_API Quaternion operator* (const Quaternion& q, float f);
FOG_API Quaternion operator/ (const Quaternion& q1, const Quaternion& q2);
FOG_API Quaternion operator* (float f, const Quaternion& q);

