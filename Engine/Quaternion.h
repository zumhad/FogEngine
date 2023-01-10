#pragma once

#include "Core.h"

#include "Vector3.h"

#include <DirectXMath.h>

class FOG_API Quaternion
{
public:
    Quaternion() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
    Quaternion(float ix, float iy, float iz, float iw) : x(ix), y(iy), z(iz), w(iw) {}
    Quaternion(const Vector3& v, float scalar) : x(v.x), y(v.y), z(v.z), w(scalar) {}
    //Quaternion(const Vector4& v) : XMFLOAT4(v.x, v.y, v.z, v.w) {}
    //Quaternion(_In_reads_(4) const float* pArray) : XMFLOAT4(pArray) {}
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

    Vector3 GetEulerAngles() const;
    void SetEulerAngles(float x, float y, float z);

    static Quaternion Euler(float x, float y, float z);
    static Quaternion FromToRotation(const Vector3& fromDir, const Vector3& toDir);
    static Quaternion LookRotation(const Vector3& forward, const Vector3& up);

    static const Quaternion& Identity();

public:
	float x, y, z, w;
};

