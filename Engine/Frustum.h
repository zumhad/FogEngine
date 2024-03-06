#pragma once

#include "Core.h"

#include "Matrix.h"
#include "Vector3.h"
#include "Vector4.h"

class FOG_API ShadowMap;

class FOG_API Frustum
{
    friend class ShadowMap;

public:
    Frustum();

private:
    void Update(Matrix& view, Matrix& proj);
    Matrix Cascade(Vector3 dir, int resolution);
    void Create(Matrix& proj);
    void Transform(Matrix& inv);
    void GetCorners(Vector3* corners);

private:
    Vector3 mOrigin;
    Vector4 mOrientation;

    float mRightSlope;
    float mLeftSlope;
    float mTopSlope;
    float mBottomSlope;
    float mNear, mFar;
};

