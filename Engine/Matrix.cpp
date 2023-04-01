#include "Matrix.h"

#include <cstring>

using namespace DirectX;

Matrix::Matrix()
{
    XMStoreFloat4x4(&m, XMMatrixIdentity());
}

Matrix::Matrix(CXMMATRIX _m)
{
    XMStoreFloat4x4(&m, _m);
}

Matrix::operator DirectX::XMMATRIX() const
{
    return XMLoadFloat4x4(&m);
}

Matrix::operator DirectX::XMFLOAT4X4() const
{
    return m;
}

Matrix& Matrix::operator+= (const Matrix& _m)
{
    XMMATRIX m1 = XMLoadFloat4x4(&m);
    XMMATRIX m2 = XMLoadFloat4x4(&_m.m);

    XMStoreFloat4x4(&m, m1 + m2);

    return *this;
}

Matrix& Matrix::operator-= (const Matrix& _m)
{
    XMMATRIX m1 = XMLoadFloat4x4(&m);
    XMMATRIX m2 = XMLoadFloat4x4(&_m.m);

    XMStoreFloat4x4(&m, m1 - m2);

    return *this;
}

Matrix& Matrix::operator*= (const Matrix& _m)
{
    XMMATRIX m1 = XMLoadFloat4x4(&m);
    XMMATRIX m2 = XMLoadFloat4x4(&_m.m);

    XMStoreFloat4x4(&m, m1 * m2);

    return *this;
}

Matrix& Matrix::operator*= (float f)
{
    XMMATRIX m1 = XMLoadFloat4x4(&m);

    XMStoreFloat4x4(&m, m1 * f);

    return *this;
}

Matrix& Matrix::operator/= (float f)
{
    assert(f != 0.0f);

    XMMATRIX m1 = XMLoadFloat4x4(&m);

    XMStoreFloat4x4(&m, m1 / f);

    return *this;
}

Matrix Matrix::operator+ () const
{
    XMMATRIX m1 = XMLoadFloat4x4(&m);

    Matrix _m;
    XMStoreFloat4x4(&_m.m, +m1);

    return _m;
}

Matrix Matrix::operator- () const
{
    XMMATRIX m1 = XMLoadFloat4x4(&m);

    Matrix _m;
    XMStoreFloat4x4(&_m.m, -m1);

    return _m;
}

Matrix Matrix::operator+ (const Matrix& _m)
{
    XMMATRIX m1 = XMLoadFloat4x4(&m);
    XMMATRIX m2 = XMLoadFloat4x4(&_m.m);

    Matrix __m;
    XMStoreFloat4x4(&__m.m, m1 + m2);

    return __m;
}

Matrix Matrix::operator- (const Matrix& _m)
{
    XMMATRIX m1 = XMLoadFloat4x4(&m);
    XMMATRIX m2 = XMLoadFloat4x4(&_m.m);

    Matrix __m;
    XMStoreFloat4x4(&__m.m, m1 - m2);

    return __m;
}

Matrix Matrix::operator* (const Matrix& _m)
{
    XMMATRIX m1 = XMLoadFloat4x4(&m);
    XMMATRIX m2 = XMLoadFloat4x4(&_m.m);

    Matrix __m;
    XMStoreFloat4x4(&__m.m, m1 * m2);

    return __m;
}

Matrix Matrix::operator* (float f)
{
    XMMATRIX m1 = XMLoadFloat4x4(&m);

    Matrix __m;
    XMStoreFloat4x4(&__m.m, m1 * f);

    return __m;
}

Matrix Matrix::operator/ (float f)
{
    assert(f != 0.0f);

    XMMATRIX m1 = XMLoadFloat4x4(&m);

    Matrix __m;
    XMStoreFloat4x4(&__m.m, m1 / f);

    return __m;
}

Matrix Matrix::Invert(Matrix _m)
{
    return XMMatrixInverse(0, XMLoadFloat4x4(&_m.m));
}
