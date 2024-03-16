#include "Frustum.h"

#include "MathHelper.h"
#include "Quaternion.h"
#include "CustomString.h"
#include "ShadowPass.h"
#include "Matrix3.h"

using namespace DirectX;

Frustum::Frustum()
{
    mOrigin = Vector3(0.0f, 0.0f, 0.0f);
    mOrientation = Quaternion(0.0f, 0.0f, 0.0f, 1.0f);

    mRightSlope = 1.0f;
    mLeftSlope = -1.0f;
    mTopSlope = 1.0f;
    mBottomSlope = -1.0f;
    mNear = 0.0f;
    mFar = 1.0f;
}

void Frustum::Update(const Matrix& view, const Matrix& proj)
{
    Create(proj);

    Matrix inv = Matrix::Inverse(view);
    Transform(inv);
}

void Frustum::GetCorners(Vector3* corners)
{
    Vector3 vRightTop = Vector3(mRightSlope, mTopSlope, 1.0f);
    Vector3 vRightBottom = Vector3(mRightSlope, mBottomSlope, 1.0f);
    Vector3 vLeftTop = Vector3(mLeftSlope, mTopSlope, 1.0f);
    Vector3 vLeftBottom = Vector3(mLeftSlope, mBottomSlope, 1.0f);
    Vector3 vNear = Vector3(mNear);
    Vector3 vFar = Vector3(mFar);

    corners[0] = vLeftTop * vNear;
    corners[1] = vRightTop * vNear;
    corners[2] = vRightBottom * vNear;
    corners[3] = vLeftBottom * vNear;
    corners[4] = vLeftTop * vFar;
    corners[5] = vRightTop * vFar;
    corners[6] = vRightBottom * vFar;
    corners[7] = vLeftBottom * vFar;

    for (size_t i = 0; i < 8; ++i)
    {
        corners[i] = Vector3::Rotate(corners[i], mOrientation) + mOrigin;
    }
}

void Frustum::Create(const Matrix& proj)
{
    static Vector4 homogenousPoints[6] =
    {
        {{{  1.0f,  0.0f, 1.0f, 1.0f }}},
        {{{ -1.0f,  0.0f, 1.0f, 1.0f }}},
        {{{  0.0f,  1.0f, 1.0f, 1.0f }}},
        {{{  0.0f, -1.0f, 1.0f, 1.0f }}},
        {{{  0.0f,  0.0f, 0.0f, 1.0f }}},
        {{{  0.0f,  0.0f, 1.0f, 1.0f }}}
    };

    Matrix matInverse = Matrix::Inverse(proj);

    Vector4 points[6];

    for (int i = 0; i < 6; ++i)
    {
        points[i] = Vector4::Transform(homogenousPoints[i], matInverse);
    }

    mOrigin = Vector3(0.0f, 0.0f, 0.0f);
    mOrientation = Quaternion(0.0f, 0.0f, 0.0f, 1.0f);

    points[0] = points[0] * Vector4::Reciprocal(Vector4(points[0].z));
    points[1] = points[1] * Vector4::Reciprocal(Vector4(points[1].z));
    points[2] = points[2] * Vector4::Reciprocal(Vector4(points[2].z));
    points[3] = points[3] * Vector4::Reciprocal(Vector4(points[3].z));

    mRightSlope = points[0].x;
    mLeftSlope = points[1].x;
    mTopSlope = points[2].y;
    mBottomSlope = points[3].y;

    points[4] = points[4] * Vector4::Reciprocal(Vector4(points[4].w));
    points[5] = points[5] * Vector4::Reciprocal(Vector4(points[5].w));

    mNear = points[4].z;
    mFar = points[5].z;
}

void Frustum::Transform(const Matrix& inv)
{
    Matrix m;
    m.v[0] = Vector4(Vector3::Normalize(inv.v[0]), 0.0f);
    m.v[1] = Vector4(Vector3::Normalize(inv.v[1]), 0.0f);
    m.v[2] = Vector4(Vector3::Normalize(inv.v[2]), 0.0f);
    m.v[3] = Vector4(0.0f, 0.0f, 0.0f, 1.0f);

    Quaternion rotation = Quaternion::Rotation(m);
    mOrientation = mOrientation * rotation;

    mOrigin = Vector3::Transform(mOrigin, inv);

    Vector3 dX = Vector3::Dot(inv.v[0], inv.v[0]);
    Vector3 dY = Vector3::Dot(inv.v[1], inv.v[1]);
    Vector3 dZ = Vector3::Dot(inv.v[2], inv.v[2]);

    Vector3 d = Vector3::Max(dX, Vector3::Max(dY, dZ));
    float scale = Math::Sqrt(d.x);

    mNear = mNear * scale;
    mFar = mFar * scale;
}
