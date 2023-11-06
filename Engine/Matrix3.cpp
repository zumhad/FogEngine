#include "Matrix3.h"

#pragma warning(disable : 26495)

#include "Utility.h"

using namespace DirectX;

Matrix3::Matrix3(const DirectX::XMFLOAT3X3& _m)
{
    std::memcpy(m, &_m, sizeof(XMFLOAT3X3));
}

Matrix3::Matrix3(const float* arr)
{
    std::memcpy(m, arr, 9);
}

Matrix3::Matrix3()
{
    _00 = 1.0f; _01 = 0.0f; _02 = 0.0f;
    _10 = 0.0f; _11 = 1.0f; _12 = 0.0f;
    _20 = 0.0f; _21 = 0.0f; _22 = 1.0f;
}

Matrix3::Matrix3(CXMMATRIX _m)
{
    XMStoreFloat3x3((XMFLOAT3X3*)m, _m);
}

Matrix3::operator DirectX::XMMATRIX() const
{
    return XMLoadFloat3x3((XMFLOAT3X3*)m);
}

Matrix3::operator DirectX::XMFLOAT3X3() const
{
    return *((XMFLOAT3X3*)m);
}

Matrix3& Matrix3::operator+= (const Matrix3& _m)
{
    XMMATRIX m1 = XMLoadFloat3x3((XMFLOAT3X3*)m);
    XMMATRIX m2 = XMLoadFloat3x3((XMFLOAT3X3*)(_m.m));

    XMStoreFloat3x3((XMFLOAT3X3*)m, m1 + m2);

    return *this;
}

Matrix3& Matrix3::operator-= (const Matrix3& _m)
{
    XMMATRIX m1 = XMLoadFloat3x3((XMFLOAT3X3*)m);
    XMMATRIX m2 = XMLoadFloat3x3((XMFLOAT3X3*)(_m.m));

    XMStoreFloat3x3((XMFLOAT3X3*)m, m1 - m2);

    return *this;
}

Matrix3& Matrix3::operator*= (const Matrix3& _m)
{
    XMMATRIX m1 = XMLoadFloat3x3((XMFLOAT3X3*)m);
    XMMATRIX m2 = XMLoadFloat3x3((XMFLOAT3X3*)(_m.m));

    XMStoreFloat3x3((XMFLOAT3X3*)m, m1 * m2);

    return *this;
}

Matrix3& Matrix3::operator*= (float f)
{
    XMMATRIX m1 = XMLoadFloat3x3((XMFLOAT3X3*)m);

    XMStoreFloat3x3((XMFLOAT3X3*)m, m1 * f);

    return *this;
}

Matrix3& Matrix3::operator/= (float f)
{
    FOG_ASSERT(f != 0);

    XMMATRIX m1 = XMLoadFloat3x3((XMFLOAT3X3*)m);

    XMStoreFloat3x3((XMFLOAT3X3*)m, m1 / f);

    return *this;
}

Matrix3 Matrix3::operator+ () const
{
    XMMATRIX m1 = XMLoadFloat3x3((XMFLOAT3X3*)m);

    Matrix3 _m;
    XMStoreFloat3x3((XMFLOAT3X3*)(_m.m), +m1);

    return _m;
}

Matrix3 Matrix3::operator- () const
{
    XMMATRIX m1 = XMLoadFloat3x3((XMFLOAT3X3*)m);

    Matrix3 _m;
    XMStoreFloat3x3((XMFLOAT3X3*)(_m.m), -m1);

    return _m;
}

Matrix3 Matrix3::operator+ (const Matrix3& _m)
{
    XMMATRIX m1 = XMLoadFloat3x3((XMFLOAT3X3*)m);
    XMMATRIX m2 = XMLoadFloat3x3((XMFLOAT3X3*)(_m.m));

    Matrix3 __m;
    XMStoreFloat3x3((XMFLOAT3X3*)(__m.m), m1 + m2);

    return __m;
}

Matrix3 Matrix3::operator- (const Matrix3& _m)
{
    XMMATRIX m1 = XMLoadFloat3x3((XMFLOAT3X3*)m);
    XMMATRIX m2 = XMLoadFloat3x3((XMFLOAT3X3*)(_m.m));

    Matrix3 __m;
    XMStoreFloat3x3((XMFLOAT3X3*)(__m.m), m1 - m2);

    return __m;
}

Matrix3 Matrix3::operator* (const Matrix3& _m)
{
    XMMATRIX m1 = XMLoadFloat3x3((XMFLOAT3X3*)m);
    XMMATRIX m2 = XMLoadFloat3x3((XMFLOAT3X3*)(_m.m));

    Matrix3 __m;
    XMStoreFloat3x3((XMFLOAT3X3*)(__m.m), m1 * m2);

    return __m;
}

Matrix3 Matrix3::operator* (float f)
{
    XMMATRIX m1 = XMLoadFloat3x3((XMFLOAT3X3*)m);

    Matrix3 _m;
    XMStoreFloat3x3((XMFLOAT3X3*)(_m.m), m1 * f);

    return _m;
}

Matrix3 Matrix3::operator/ (float f)
{
    FOG_ASSERT(f != 0.0f);

    XMMATRIX m1 = XMLoadFloat3x3((XMFLOAT3X3*)m);

    Matrix3 _m;
    XMStoreFloat3x3((XMFLOAT3X3*)(_m.m), m1 / f);

    return _m;
}

Matrix3 Matrix3::Inverse(Matrix3& _m)
{
    XMMATRIX m1 = XMLoadFloat3x3((XMFLOAT3X3*)(_m.m));

    Matrix3 __m;
    XMStoreFloat3x3((XMFLOAT3X3*)(__m.m), XMMatrixInverse(0, m1));

    return __m;
}
