#pragma once

#pragma warning(push)
#pragma warning(disable : 4201)

#include "Core.h"

#include "Vector4.h"

#include <DirectXMath.h>

class FOG_API Matrix
{
public:
    Matrix();

    Matrix(const DirectX::XMFLOAT4X4& _m);
    Matrix(const float* arr);
    Matrix(DirectX::CXMMATRIX _m);

    operator DirectX::XMMATRIX() const;
    operator DirectX::XMFLOAT4X4() const;

    Matrix& operator+= (const Matrix& _m);
    Matrix& operator-= (const Matrix& _m);
    Matrix& operator*= (const Matrix& _m);
    Matrix& operator*= (float f);
    Matrix& operator/= (float f);

    Matrix operator+ (const Matrix& _m);
    Matrix operator- (const Matrix& _m);
    Matrix operator* (const Matrix& _m);
    Matrix operator* (float f);
    Matrix operator/ (float f);

    Matrix operator+ () const;
    Matrix operator- () const;

    static Matrix Inverse(const Matrix& _m);
    static Matrix Scaling(const Vector3& _v);
    static Matrix Scaling(float x, float y, float z);
    static Matrix Translation(const Vector3& _v);
    static Matrix Translation(float x, float y, float z);
    static Matrix AffineTransformation(const Vector3& position, const Vector3& rotation, const Vector3& scale);

public:
    union
    {
        struct
        {
            float _00, _01, _02, _03;
            float _10, _11, _12, _13;
            float _20, _21, _22, _23;
            float _30, _31, _32, _33;
        };
        float m[4][4];
        Vector4 v[4];
    };
};

#pragma warning(pop)