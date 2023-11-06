#include "Frustum.h"

#include "MathHelper.h"
#include "Quaternion.h"
#include "CustomString.h"
#include "ShadowMap.h"

#include <d3d11.h>
#include <limits>

using namespace DirectX;
using namespace std;

Frustum::Frustum()
{
    mOrigin = Vector3(0.0f, 0.0f, 0.0f);
    mOrientation = Vector4(0.0f, 0.0f, 0.0f, 1.0f);

    mRightSlope = 1.0f;
    mLeftSlope = -1.0f;
    mTopSlope = 1.0f;
    mBottomSlope = -1.0f;
    mNear = 0.0f;
    mFar = 1.0f;
}

void Frustum::Update(Matrix& view, Matrix& proj)
{
    Create(proj);

    Matrix inv = XMMatrixInverse(0, view);
    Transform(inv);
}

Matrix Frustum::Cascade(Vector3 dir)
{
    static Vector3 staticDir = Vector3(0.0f, 0.0f, 0.0f);

    Vector3 corner[8];
    GetCorners(corner);

    Vector3 center;
    for (int i = 0; i < 8; i++)
    {
        center += corner[i];
    }
    center /= 8.0f;

    static Matrix lightView;
    if (dir != staticDir)
    {
        Vector3 rot;
        rot.x = -Math::ASin(dir.y);
        rot.y = Math::ATan2(dir.x, dir.z);
        Quaternion q = XMQuaternionRotationRollPitchYawFromVector(rot);
        Vector3 up = XMVector3Rotate(XMVectorSet(0, 1, 0, 1), q);

        lightView = XMMatrixLookAtLH(center + dir, center, up);
        staticDir = dir;
    }

    Vector3 min = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
    Vector3 max = Vector3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

    for (int i = 0; i < 8; i++)
    {
        Vector3 v = XMVector3Transform(corner[i], lightView);

        min = XMVectorMin(min, v);
        max = XMVectorMax(max, v);
    }

    float nearZ = min.z;
    float farZ = max.z;

    Vector3 diagonal = corner[0] - corner[6];
    diagonal = XMVector3Length(diagonal);

    float cascadeBound = diagonal.x;

    Vector3 boarderOffset = (diagonal - (max - min)) * Vector3(0.5f, 0.5f, 0.5f);
    boarderOffset.z = 0.0f;

    max += boarderOffset;
    min -= boarderOffset;

    float worldUnitsPerTexel = cascadeBound / ShadowMap::GetResolution();

    min /= worldUnitsPerTexel;
    min = XMVectorFloor(min);
    min *= worldUnitsPerTexel;

    max /= worldUnitsPerTexel;
    max = XMVectorFloor(max);
    max *= worldUnitsPerTexel;

    Matrix lightProj = XMMatrixOrthographicOffCenterLH(min.x, max.x, min.y, max.y, nearZ, farZ);

    return lightView * lightProj;
}

void Frustum::GetCorners(Vector3* corners)
{
    XMVECTOR vRightTop = XMVectorSet(mRightSlope, mTopSlope, 1.0f, 0.0f);
    XMVECTOR vRightBottom = XMVectorSet(mRightSlope, mBottomSlope, 1.0f, 0.0f);
    XMVECTOR vLeftTop = XMVectorSet(mLeftSlope, mTopSlope, 1.0f, 0.0f);
    XMVECTOR vLeftBottom = XMVectorSet(mLeftSlope, mBottomSlope, 1.0f, 0.0f);
    XMVECTOR vNear = XMVectorReplicatePtr(&mNear);
    XMVECTOR vFar = XMVectorReplicatePtr(&mFar);

    corners[0] = XMVectorMultiply(vLeftTop, vNear);
    corners[1] = XMVectorMultiply(vRightTop, vNear);
    corners[2] = XMVectorMultiply(vRightBottom, vNear);
    corners[3] = XMVectorMultiply(vLeftBottom, vNear);
    corners[4] = XMVectorMultiply(vLeftTop, vFar);
    corners[5] = XMVectorMultiply(vRightTop, vFar);
    corners[6] = XMVectorMultiply(vRightBottom, vFar);
    corners[7] = XMVectorMultiply(vLeftBottom, vFar);

    for (size_t i = 0; i < 8; ++i)
    {
        corners[i] = XMVectorAdd(XMVector3Rotate(corners[i], mOrientation), mOrigin);
    }
}

void Frustum::Create(Matrix& proj)
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

    XMMATRIX matInverse = XMMatrixInverse(0, proj);

    XMVECTOR points[6];

    for (int i = 0; i < 6; ++i)
    {
        points[i] = XMVector4Transform(homogenousPoints[i], matInverse);
    }

    mOrigin = Vector3(0.0f, 0.0f, 0.0f);
    mOrientation = Vector4(0.0f, 0.0f, 0.0f, 1.0f);

    points[0] = XMVectorMultiply(points[0], XMVectorReciprocal(XMVectorSplatZ(points[0])));
    points[1] = XMVectorMultiply(points[1], XMVectorReciprocal(XMVectorSplatZ(points[1])));
    points[2] = XMVectorMultiply(points[2], XMVectorReciprocal(XMVectorSplatZ(points[2])));
    points[3] = XMVectorMultiply(points[3], XMVectorReciprocal(XMVectorSplatZ(points[3])));

    mRightSlope = XMVectorGetX(points[0]);
    mLeftSlope = XMVectorGetX(points[1]);
    mTopSlope = XMVectorGetY(points[2]);
    mBottomSlope = XMVectorGetY(points[3]);

    points[4] = XMVectorMultiply(points[4], XMVectorReciprocal(XMVectorSplatW(points[4])));
    points[5] = XMVectorMultiply(points[5], XMVectorReciprocal(XMVectorSplatW(points[5])));

    mNear = XMVectorGetZ(points[4]);
    mFar = XMVectorGetZ(points[5]);
}

void Frustum::Transform(Matrix& inv)
{
    Vector3 r0 = Vector3(inv.m[0][0], inv.m[0][1], inv.m[0][2]);
    Vector3 r1 = Vector3(inv.m[1][0], inv.m[1][1], inv.m[1][2]);
    Vector3 r2 = Vector3(inv.m[2][0], inv.m[2][1], inv.m[2][2]);

    Vector3 n0 = XMVector3Normalize(r0);
    Vector3 n1 = XMVector3Normalize(r1);
    Vector3 n2 = XMVector3Normalize(r2);

    Matrix m;
    m.m[0][0] = r0.x;
    m.m[0][1] = r0.y;
    m.m[0][2] = r0.z;
    m.m[1][0] = r1.x;
    m.m[1][1] = r1.y;
    m.m[1][2] = r1.z;
    m.m[2][0] = r2.x;
    m.m[2][1] = r2.y;
    m.m[2][2] = r2.z;

    XMVECTOR rotation = XMQuaternionRotationMatrix(m);
    mOrientation = XMQuaternionMultiply(mOrientation, rotation);

    mOrigin = XMVector3Transform(mOrigin, inv);

    XMVECTOR dX = XMVector3Dot(r0, r0);
    XMVECTOR dY = XMVector3Dot(r1, r1);
    XMVECTOR dZ = XMVector3Dot(r2, r2);

    XMVECTOR d = XMVectorMax(dX, XMVectorMax(dY, dZ));
    float scale = Math::Sqrt(XMVectorGetX(d));

    mNear = mNear * scale;
    mFar = mFar * scale;
}
