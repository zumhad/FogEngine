#pragma once

#pragma warning(push)
#pragma warning(disable : 4201)

#include "Core.h"

#include <DirectXMath.h>

//struct FOG_API DirectX::XMFLOAT4X4;

class FOG_API Matrix
{
public:
    Matrix();

    Matrix(const DirectX::XMFLOAT4X4& _m);
    Matrix(const float* arr);
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

    static Matrix Invert(Matrix& _m);

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
    };
};

#pragma warning(pop)