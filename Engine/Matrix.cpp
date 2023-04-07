#include "Matrix.h"

#pragma warning(disable : 26495)

#include <cstring>

using namespace DirectX;

Matrix::Matrix(const DirectX::XMFLOAT4X4& _m)
{
    std::memcpy(m, &_m, sizeof(XMFLOAT4X4));
}

Matrix::Matrix(const float* arr)
{
    std::memcpy(m, arr, 16);
}

Matrix::Matrix()
{
    _00 = 1.0f; _01 = 0.0f; _02 = 0.0f; _03 = 0.0f;
    _10 = 0.0f; _11 = 1.0f; _12 = 0.0f; _13 = 0.0f;
    _20 = 0.0f; _21 = 0.0f; _22 = 1.0f; _23 = 0.0f;
    _30 = 0.0f; _31 = 0.0f; _32 = 0.0f; _33 = 1.0f;
}

Matrix::Matrix(CXMMATRIX _m)
{
    XMStoreFloat4x4((XMFLOAT4X4*)m, _m);
}

Matrix::operator DirectX::XMMATRIX() const
{
    return XMLoadFloat4x4((XMFLOAT4X4*)m);
}

Matrix::operator DirectX::XMFLOAT4X4() const
{
    return *((XMFLOAT4X4*)m);
}

Matrix& Matrix::operator+= (const Matrix& _m)
{
    XMMATRIX m1 = XMLoadFloat4x4((XMFLOAT4X4*)m);
    XMMATRIX m2 = XMLoadFloat4x4((XMFLOAT4X4*)(_m.m));

    XMStoreFloat4x4((XMFLOAT4X4*)m, m1 + m2);

    return *this;
}

Matrix& Matrix::operator-= (const Matrix& _m)
{
    XMMATRIX m1 = XMLoadFloat4x4((XMFLOAT4X4*)m);
    XMMATRIX m2 = XMLoadFloat4x4((XMFLOAT4X4*)(_m.m));

    XMStoreFloat4x4((XMFLOAT4X4*)m, m1 - m2);

    return *this;
}

Matrix& Matrix::operator*= (const Matrix& _m)
{
    XMMATRIX m1 = XMLoadFloat4x4((XMFLOAT4X4*)m);
    XMMATRIX m2 = XMLoadFloat4x4((XMFLOAT4X4*)(_m.m));

    XMStoreFloat4x4((XMFLOAT4X4*)m, m1 * m2);

    return *this;
}

Matrix& Matrix::operator*= (float f)
{
    XMMATRIX m1 = XMLoadFloat4x4((XMFLOAT4X4*)m);

    XMStoreFloat4x4((XMFLOAT4X4*)m, m1 * f);

    return *this;
}

Matrix& Matrix::operator/= (float f)
{
    FOG_ASSERT(f != 0);

    XMMATRIX m1 = XMLoadFloat4x4((XMFLOAT4X4*)m);

    XMStoreFloat4x4((XMFLOAT4X4*)m, m1 / f);

    return *this;
}

Matrix Matrix::operator+ () const
{
    XMMATRIX m1 = XMLoadFloat4x4((XMFLOAT4X4*)m);

    Matrix _m;
    XMStoreFloat4x4((XMFLOAT4X4*)(_m.m), +m1);

    return _m;
}

Matrix Matrix::operator- () const
{
    XMMATRIX m1 = XMLoadFloat4x4((XMFLOAT4X4*)m);

    Matrix _m;
    XMStoreFloat4x4((XMFLOAT4X4*)(_m.m), -m1);

    return _m;
}

Matrix Matrix::operator+ (const Matrix& _m)
{
    XMMATRIX m1 = XMLoadFloat4x4((XMFLOAT4X4*)m);
    XMMATRIX m2 = XMLoadFloat4x4((XMFLOAT4X4*)(_m.m));

    Matrix __m;
    XMStoreFloat4x4((XMFLOAT4X4*)(__m.m), m1 + m2);

    return __m;
}

Matrix Matrix::operator- (const Matrix& _m)
{
    XMMATRIX m1 = XMLoadFloat4x4((XMFLOAT4X4*)m);
    XMMATRIX m2 = XMLoadFloat4x4((XMFLOAT4X4*)(_m.m));

    Matrix __m;
    XMStoreFloat4x4((XMFLOAT4X4*)(__m.m), m1 - m2);

    return __m;
}

Matrix Matrix::operator* (const Matrix& _m)
{
    XMMATRIX m1 = XMLoadFloat4x4((XMFLOAT4X4*)m);
    XMMATRIX m2 = XMLoadFloat4x4((XMFLOAT4X4*)(_m.m));

    Matrix __m;
    XMStoreFloat4x4((XMFLOAT4X4*)(__m.m), m1 * m2);

    return __m;
}

Matrix Matrix::operator* (float f)
{
    XMMATRIX m1 = XMLoadFloat4x4((XMFLOAT4X4*)m);

    Matrix _m;
    XMStoreFloat4x4((XMFLOAT4X4*)(_m.m), m1 * f);

    return _m;
}

Matrix Matrix::operator/ (float f)
{
    FOG_ASSERT(f != 0.0f);

    XMMATRIX m1 = XMLoadFloat4x4((XMFLOAT4X4*)m);

    Matrix _m;
    XMStoreFloat4x4((XMFLOAT4X4*)(_m.m), m1 / f);

    return _m;
}

Matrix Matrix::Invert(Matrix& _m)
{
    XMMATRIX m1 = XMLoadFloat4x4((XMFLOAT4X4*)(_m.m));

    Matrix __m;
    XMStoreFloat4x4((XMFLOAT4X4*)(__m.m), XMMatrixInverse(0, m1));

    return __m;
}
