#include "Matrix.h"

#include <cstring>

using namespace DirectX;

Matrix::Matrix(const XMFLOAT4X4& _m)
{ 
	std::memcpy(&m, &_m, sizeof(XMFLOAT4X4));
}

Matrix::Matrix(const float* arr)
{
    std::memcpy(&m, &arr, 16 * sizeof(float));
}

Matrix::Matrix(CXMMATRIX _m)
{
    XMFLOAT4X4 f;
    XMStoreFloat4x4(&f, _m);
    std::memcpy(&m, &f, sizeof(XMFLOAT4X4));
}

Matrix::operator DirectX::XMMATRIX() const
{
    XMFLOAT4X4 f;
    std::memcpy(&f, &m, sizeof(XMFLOAT4X4));
    return XMLoadFloat4x4(&f);
}

Matrix::operator DirectX::XMFLOAT4X4() const
{
    XMFLOAT4X4 f;
    std::memcpy(&f, &m, sizeof(XMFLOAT4X4));
    return f;
}

Matrix& Matrix::operator+= (const Matrix& _m)
{
    XMVECTOR v1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m[0]));
    XMVECTOR v2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m[1]));
    XMVECTOR v3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m[2]));
    XMVECTOR v4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m[3]));

    XMVECTOR v5 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_m.m[0]));
    XMVECTOR v6 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_m.m[1]));
    XMVECTOR v7 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_m.m[2]));
    XMVECTOR v8 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_m.m[3]));

    v1 = XMVectorAdd(v1, v5);
    v2 = XMVectorAdd(v2, v6);
    v3 = XMVectorAdd(v3, v7);
    v4 = XMVectorAdd(v4, v8);

    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&m[0]), v1);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&m[1]), v2);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&m[2]), v3);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&m[3]), v4);
    return *this;
}

Matrix& Matrix::operator-= (const Matrix& _m)
{
    XMVECTOR v1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m[0]));
    XMVECTOR v2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m[1]));
    XMVECTOR v3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m[2]));
    XMVECTOR v4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m[3]));

    XMVECTOR v5 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_m.m[0]));
    XMVECTOR v6 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_m.m[1]));
    XMVECTOR v7 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_m.m[2]));
    XMVECTOR v8 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_m.m[3]));

    v1 = XMVectorSubtract(v1, v5);
    v2 = XMVectorSubtract(v2, v6);
    v3 = XMVectorSubtract(v3, v7);
    v4 = XMVectorSubtract(v4, v8);

    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&m[0]), v1);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&m[1]), v2);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&m[2]), v3);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&m[3]), v4);
    return *this;
}

Matrix& Matrix::operator*= (const Matrix& _m)
{
    *this = XMMatrixMultiply(*this, _m);
    return *this;
}

Matrix& Matrix::operator*= (float f)
{
    XMVECTOR v1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m[0]));
    XMVECTOR v2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m[1]));
    XMVECTOR v3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m[2]));
    XMVECTOR v4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m[3]));

    v1 = XMVectorScale(v1, f);
    v2 = XMVectorScale(v2, f);
    v3 = XMVectorScale(v3, f);
    v4 = XMVectorScale(v4, f);

    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&m[0]), v1);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&m[1]), v2);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&m[2]), v3);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&m[3]), v4);
    return *this;
}

Matrix& Matrix::operator/= (float f)
{
    assert(f != 0.0f);
    XMVECTOR v1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m[0]));
    XMVECTOR v2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m[1]));
    XMVECTOR v3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m[2]));
    XMVECTOR v4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m[3]));

    const float rs = 1.0f / f;

    v1 = XMVectorScale(v1, rs);
    v2 = XMVectorScale(v2, rs);
    v3 = XMVectorScale(v3, rs);
    v4 = XMVectorScale(v4, rs);

    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&m[0]), v1);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&m[1]), v2);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&m[2]), v3);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&m[3]), v4);
    return *this;
}

Matrix Matrix::operator+ () const
{
    return *this;
}

Matrix Matrix::operator- () const
{
    XMVECTOR v1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m[0]));
    XMVECTOR v2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m[1]));
    XMVECTOR v3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m[2]));
    XMVECTOR v4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m[3]));

    v1 = XMVectorNegate(v1);
    v2 = XMVectorNegate(v2);
    v3 = XMVectorNegate(v3);
    v4 = XMVectorNegate(v4);

    Matrix _m;
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_m.m[0]), v1);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_m.m[1]), v2);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_m.m[2]), v3);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_m.m[3]), v4);
    return _m;
}

Matrix operator+ (const Matrix& m1, const Matrix& m2)
{
    XMVECTOR v1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m1.m[0]));
    XMVECTOR v2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m1.m[1]));
    XMVECTOR v3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m1.m[2]));
    XMVECTOR v4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m1.m[3]));

    XMVECTOR v5 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m2.m[0]));
    XMVECTOR v6 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m2.m[1]));
    XMVECTOR v7 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m2.m[2]));
    XMVECTOR v8 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m2.m[3]));

    v1 = XMVectorAdd(v1, v5);
    v2 = XMVectorAdd(v2, v6);
    v3 = XMVectorAdd(v3, v7);
    v4 = XMVectorAdd(v4, v8);

    Matrix _m;
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_m.m[0]), v1);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_m.m[1]), v2);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_m.m[2]), v3);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_m.m[3]), v4);
    return _m;
}

Matrix operator- (const Matrix& m1, const Matrix& m2)
{
    XMVECTOR v1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m1.m[0]));
    XMVECTOR v2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m1.m[1]));
    XMVECTOR v3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m1.m[2]));
    XMVECTOR v4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m1.m[3]));

    XMVECTOR v5 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m2.m[0]));
    XMVECTOR v6 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m2.m[1]));
    XMVECTOR v7 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m2.m[2]));
    XMVECTOR v8 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m2.m[3]));

    v1 = XMVectorSubtract(v1, v5);
    v2 = XMVectorSubtract(v2, v6);
    v3 = XMVectorSubtract(v3, v7);
    v4 = XMVectorSubtract(v4, v8);

    Matrix _m;
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_m.m[0]), v1);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_m.m[1]), v2);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_m.m[2]), v3);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_m.m[3]), v4);
    return _m;
}

Matrix operator* (const Matrix& m1, const Matrix& m2)
{
    Matrix _m = XMMatrixMultiply(m1, m2);
    return _m;
}

Matrix operator* (const Matrix& m, float f)
{
    XMVECTOR v1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m.m[0]));
    XMVECTOR v2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m.m[1]));
    XMVECTOR v3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m.m[2]));
    XMVECTOR v4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m.m[3]));

    v1 = XMVectorScale(v1, f);
    v2 = XMVectorScale(v2, f);
    v3 = XMVectorScale(v3, f);
    v4 = XMVectorScale(v4, f);

    Matrix _m;
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_m.m[0]), v1);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_m.m[1]), v2);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_m.m[2]), v3);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_m.m[3]), v4);
    return _m;
}

Matrix operator/ (const Matrix& m, float f)
{
    assert(f != 0.0f);

    XMVECTOR v1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m.m[0]));
    XMVECTOR v2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m.m[1]));
    XMVECTOR v3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m.m[2]));
    XMVECTOR v4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m.m[3]));

    const float rf = 1.0f / f;

    v1 = XMVectorScale(v1, rf);
    v2 = XMVectorScale(v2, rf);
    v3 = XMVectorScale(v3, rf);
    v4 = XMVectorScale(v4, rf);

    Matrix _m;
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_m.m[0]), v1);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_m.m[1]), v2);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_m.m[2]), v3);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_m.m[3]), v4);
    return _m;
}

Matrix operator/ (const Matrix& m1, const Matrix& m2)
{
    XMVECTOR v1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m1.m[0]));
    XMVECTOR v2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m1.m[1]));
    XMVECTOR v3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m1.m[2]));
    XMVECTOR v4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m1.m[3]));

    XMVECTOR v5 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m2.m[0]));
    XMVECTOR v6 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m2.m[1]));
    XMVECTOR v7 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m2.m[2]));
    XMVECTOR v8 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m2.m[3]));

    v1 = XMVectorDivide(v1, v5);
    v2 = XMVectorDivide(v2, v6);
    v3 = XMVectorDivide(v3, v7);
    v4 = XMVectorDivide(v4, v8);

    Matrix _m;
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_m.m[0]), v1);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_m.m[1]), v2);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_m.m[2]), v3);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_m.m[3]), v4);
    return _m;
}

Matrix operator* (float f, const Matrix& m)
{
    XMVECTOR v1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m.m[0]));
    XMVECTOR v2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m.m[1]));
    XMVECTOR v3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m.m[2]));
    XMVECTOR v4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m.m[3]));

    v1 = XMVectorScale(v1, f);
    v2 = XMVectorScale(v2, f);
    v3 = XMVectorScale(v3, f);
    v4 = XMVectorScale(v4, f);

    Matrix _m;
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_m.m[0]), v1);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_m.m[1]), v2);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_m.m[2]), v3);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_m.m[3]), v4);
    return _m;
}

Vector4 operator* (const Matrix& _m, const Vector4& v)
{
    return XMVector4Transform(v, _m);
}

Matrix Matrix::Invert(Matrix _m)
{
    return XMMatrixInverse(0, _m);
}
