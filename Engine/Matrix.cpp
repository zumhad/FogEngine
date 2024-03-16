#include "Matrix.h"

#pragma warning(disable : 26495)

#include "Utility.h"
#include "Quaternion.h"

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
    *this = XMMATRIX(*this) + XMMATRIX(_m);

    return *this;
}

Matrix& Matrix::operator-= (const Matrix& _m)
{
    *this = XMMATRIX(*this) - XMMATRIX(_m);

    return *this;
}

Matrix& Matrix::operator*= (const Matrix& _m)
{
    *this = XMMATRIX(*this) * XMMATRIX(_m);

    return *this;
}

Matrix& Matrix::operator*= (float f)
{
    *this = XMMATRIX(*this) * f;

    return *this;
}

Matrix& Matrix::operator/= (float f)
{
    FOG_ASSERT(f != 0);

    *this = XMMATRIX(*this) / f;

    return *this;
}

Matrix Matrix::operator+ () const
{
    return +XMMATRIX(*this);
}

Matrix Matrix::operator- () const
{
    return -XMMATRIX(*this);
}

Matrix Matrix::operator+ (const Matrix& _m)
{
    return XMMATRIX(*this) + XMMATRIX(_m);
}

Matrix Matrix::operator- (const Matrix& _m)
{
    return XMMATRIX(*this) - XMMATRIX(_m);
}

Matrix Matrix::operator* (const Matrix& _m)
{
    return XMMATRIX(*this) * XMMATRIX(_m);
}

Matrix Matrix::operator* (float f)
{
    return XMMATRIX(*this) * f;
}

Matrix Matrix::operator/ (float f)
{
    FOG_ASSERT(f != 0.0f);

    return XMMATRIX(*this) / f;
}

Matrix Matrix::Inverse(const Matrix& _m)
{
    return XMMatrixInverse(0, _m);
}

Matrix Matrix::Scaling(const Vector3& _v)
{
    return XMMatrixScalingFromVector(_v);
}

Matrix Matrix::Scaling(float x, float y, float z)
{
    return XMMatrixScaling(x, y, z);
}

Matrix Matrix::Translation(const Vector3& _v)
{
    return XMMatrixTranslationFromVector(_v);
}

Matrix Matrix::AffineTransformation(const Vector3& position, const Vector3& rotation, const Vector3& scale)
{
    Quaternion q = Quaternion::Euler(rotation);
    return XMMatrixAffineTransformation(scale, Quaternion::Identity(), q, position);
}

Matrix Matrix::Translation(float x, float y, float z)
{
    return XMMatrixTranslation(x, y, z);
}