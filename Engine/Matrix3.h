#pragma once

#pragma warning(push)
#pragma warning(disable : 4201)

#include "Core.h"

#include "Vector3.h"

#include <DirectXMath.h>

class FOG_API Matrix3
{
public:
    Matrix3();

    Matrix3(const DirectX::XMFLOAT3X3& _m);
    Matrix3(const float* arr);
    Matrix3(DirectX::CXMMATRIX _m);

    operator DirectX::XMMATRIX() const;
    operator DirectX::XMFLOAT3X3() const;

    Matrix3& operator+= (const Matrix3& _m);
    Matrix3& operator-= (const Matrix3& _m);
    Matrix3& operator*= (const Matrix3& _m);
    Matrix3& operator*= (float f);
    Matrix3& operator/= (float f);

    Matrix3 operator+ (const Matrix3& _m);
    Matrix3 operator- (const Matrix3& _m);
    Matrix3 operator* (const Matrix3& _m);
    Matrix3 operator* (float f);
    Matrix3 operator/ (float f);

    Matrix3 operator+ () const;
    Matrix3 operator- () const;

    static Matrix3 Inverse(const Matrix3& _m);

public:
    union
    {
        struct
        {
            float _00, _01, _02;
            float _10, _11, _12;
            float _20, _21, _22;
        };
        float m[3][3];
        Vector3 v[3];
    };
};

#pragma warning(pop)