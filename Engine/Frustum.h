#pragma once

#include "Core.h"

#include "Matrix.h"
#include "Vector3.h"
#include "Quaternion.h"

class FOG_API ShadowPass;

class FOG_API Frustum
{
    friend class ShadowPass;

public:
    Frustum();

private:
    void Update(const Matrix& view, const Matrix& proj);
    Matrix Cascade(const Vector3& dir, int resolution);
    void Create(const Matrix& proj);
    void Transform(const Matrix& inv);
    void GetCorners(Vector3* corners);

private:
    Vector3 mOrigin;
    Quaternion mOrientation;

    float mRightSlope;
    float mLeftSlope;
    float mTopSlope;
    float mBottomSlope;
    float mNear, mFar;
};

