#pragma once

#include "Core.h"
#include "MathHelper.h"
#include "BoundingPlane.h"
#include "BoundingBox.h"

class FOG_API Frustum
{
public:
    Frustum() {}
    Frustum(Matrix4& mat);

    bool IntersectBoundingBox(const BoundingBox& aabb);

    friend Frustum operator* (const OrthogonalTransform& xform, const Frustum& frustum);

private:
    void ConstructPerspectiveFrustum(float HTan, float VTan, float NearClip, float FarClip);

private:
	Vector3 mCorners[8];		// the corners of the frustum
	BoundingPlane mPlanes[6];			// the bounding planes
};

inline Frustum operator* (const OrthogonalTransform& xform, const Frustum& frustum)
{
    Frustum result;

    for (int i = 0; i < 8; ++i)
        result.mCorners[i] = xform * frustum.mCorners[i];

    for (int i = 0; i < 6; ++i)
        result.mPlanes[i] = xform * frustum.mPlanes[i];

    return result;
}
