#pragma once

#include "Core.h"

#include <DirectXMath.h>

#include "Vector3.h"
#include "Vector4.h"

struct FOG_API DirectX::XMFLOAT4X4;

class FOG_API Matrix
{
public:
    Matrix();

    Matrix(const DirectX::XMFLOAT4X4& _m) : m(_m) {};
    Matrix(const float* arr) : m(DirectX::XMFLOAT4X4(arr)) {};
    Matrix(DirectX::CXMMATRIX _m);

    operator DirectX::XMMATRIX() const;
    operator DirectX::XMFLOAT4X4() const;

    bool operator == (const Matrix& _m) const;
    bool operator != (const Matrix& _m) const;

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

    static Matrix Invert(Matrix _m);

public:
    DirectX::XMFLOAT4X4 m;
};