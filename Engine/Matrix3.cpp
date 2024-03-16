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
    *this = XMMATRIX(*this) + XMMATRIX(_m);

    return *this;
}

Matrix3& Matrix3::operator-= (const Matrix3& _m)
{
    *this = XMMATRIX(*this) - XMMATRIX(_m);

    return *this;
}

Matrix3& Matrix3::operator*= (const Matrix3& _m)
{
    *this = XMMATRIX(*this) * XMMATRIX(_m);

    return *this;
}

Matrix3& Matrix3::operator*= (float f)
{
    *this = XMMATRIX(*this) * f;

    return *this;
}

Matrix3& Matrix3::operator/= (float f)
{
    FOG_ASSERT(f != 0.0f);

    *this = XMMATRIX(*this) / f;

    return *this;
}

Matrix3 Matrix3::operator+ () const
{
    return +XMMATRIX(*this);
}

Matrix3 Matrix3::operator- () const
{
    return -XMMATRIX(*this);
}

Matrix3 Matrix3::operator+ (const Matrix3& _m)
{
    return XMMATRIX(*this) + XMMATRIX(_m);
}

Matrix3 Matrix3::operator- (const Matrix3& _m)
{
    return XMMATRIX(*this) - XMMATRIX(_m);
}

Matrix3 Matrix3::operator* (const Matrix3& _m)
{
    return XMMATRIX(*this) * XMMATRIX(_m);
}

Matrix3 Matrix3::operator* (float f)
{
    return XMMATRIX(*this) * f;
}

Matrix3 Matrix3::operator/ (float f)
{
    FOG_ASSERT(f != 0.0f);

    return XMMATRIX(*this) / f;
}

Matrix3 Matrix3::Inverse(const Matrix3& _m)
{
    return XMMatrixInverse(0, _m);
}
