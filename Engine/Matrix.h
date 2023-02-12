#pragma once

#include "Core.h"

#include <DirectXMath.h>

#include "Vector3.h"
#include "Vector4.h"

class FOG_API Matrix
{
public:
    Matrix() : m{ 1.0f, 0.0f, 0.0f, 0.0f,
                  0.0f, 1.0f, 0.0f, 0.0f,
                  0.0f, 0.0f, 1.0f, 0.0f,
                  0.0f, 0.0f, 0.0f, 1.0f } {}

    Matrix(float m00, float m01, float m02, float m03,
           float m10, float m11, float m12, float m13,
           float m20, float m21, float m22, float m23,
           float m30, float m31, float m32, float m33) : 
        m{ m00, m01, m02, m03,
           m10, m11, m12, m13,
           m20, m21, m22, m23,
           m30, m31, m32, m33 } {}

    Matrix(const Vector3& v0, const Vector3& v1, const Vector3& v2) :
        m{ v0.x, v0.y, v0.z, 0.0f,
           v1.x, v1.y, v1.z, 0.0f,
           v2.x, v2.y, v2.z, 0.0f,
           0.0f, 0.0f, 0.0f, 1.0f } {}

    Matrix(const Vector4& v0, const Vector4& v1, const Vector4& v2, const Vector4& v3) :
        m{ v0.x, v0.y, v0.z, v0.w,
           v1.x, v1.y, v1.z, v1.w,
           v2.x, v2.y, v2.z, v2.w,
           v3.x, v3.y, v3.z, v3.w } {}

    Matrix(const DirectX::XMFLOAT4X4& _m);
    Matrix(DirectX::CXMMATRIX _m);
    Matrix(const float* arr);

    operator DirectX::XMMATRIX() const;
    operator DirectX::XMFLOAT4X4() const;

    bool operator == (const Matrix& _m) const;
    bool operator != (const Matrix& _m) const;

    Matrix& operator+= (const Matrix& _m);
    Matrix& operator-= (const Matrix& _m);
    Matrix& operator*= (const Matrix& _m);
    Matrix& operator*= (float f);
    Matrix& operator/= (float f);
    Matrix& operator/= (const Matrix& _m);

    Matrix operator+ () const;
    Matrix operator- () const;

    static Matrix Invert(Matrix _m);

public:
    float m[4][4];
};

FOG_API Matrix operator+ (const Matrix& m1, const Matrix& m2);
FOG_API Matrix operator- (const Matrix& m1, const Matrix& m2);
FOG_API Matrix operator* (const Matrix& m1, const Matrix& m2);
FOG_API Matrix operator* (const Matrix& _m, float f);
FOG_API Matrix operator/ (const Matrix& _m, float f);
FOG_API Matrix operator/ (const Matrix& m1, const Matrix& m2);
FOG_API Matrix operator* (float f, const Matrix& _m);
FOG_API Vector4 operator* (const Matrix& _m, const Vector4& v);

