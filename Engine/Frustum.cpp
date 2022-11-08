#include "Frustum.h"

Frustum::Frustum(Matrix4& mat)
{
    const float* ProjMatF = (const float*)&mat;

    const float RcpXX = 1.0f / ProjMatF[0];
    const float RcpYY = 1.0f / ProjMatF[5];
    const float RcpZZ = 1.0f / ProjMatF[10];


    float NearClip = ProjMatF[14] * -RcpZZ;
    float FarClip = (NearClip * ProjMatF[10]) / (ProjMatF[10] - 1.0f);

    ConstructPerspectiveFrustum(RcpXX, RcpYY, NearClip, FarClip);
}

bool Frustum::IntersectBoundingBox(const BoundingBox& aabb)
{
    for (int i = 0; i < 6; ++i)
    {
        BoundingPlane p = mPlanes[i];
        Vector3 farCorner = Select(aabb.GetMin(), aabb.GetMax(), p.GetNormal() > Vector3::Zero());
        if (p.DistanceFromPoint(farCorner) < 0.0f)
            return false;
    }

    return true;
}

void Frustum::ConstructPerspectiveFrustum(float HTan, float VTan, float NearClip, float FarClip)
{
    const float NearX = HTan * NearClip;
    const float NearY = VTan * NearClip;
    const float FarX = HTan * FarClip;
    const float FarY = VTan * FarClip;

    mCorners[0] = Vector3(-NearX, -NearY, NearClip);	// Near lower left
    mCorners[1] = Vector3(-NearX, NearY, NearClip);	// Near upper left
    mCorners[2] = Vector3(NearX, -NearY, NearClip);	// Near lower right
    mCorners[3] = Vector3(NearX, NearY, NearClip);	// Near upper right
    mCorners[4] = Vector3(-FarX, -FarY, FarClip);	// Far lower left
    mCorners[5] = Vector3(-FarX, FarY, FarClip);	// Far upper left
    mCorners[6] = Vector3(FarX, -FarY, FarClip);	// Far lower right
    mCorners[7] = Vector3(FarX, FarY, FarClip);	// Far upper right

    const float NHx = RecipSqrt(1.0f + HTan * HTan);
    const float NHz = -NHx * HTan;
    const float NVy = RecipSqrt(1.0f + VTan * VTan);
    const float NVz = -NVy * VTan;

    // Define the bounding planes
    mPlanes[0] = BoundingPlane(0.0f, 0.0f, 1.0f, -NearClip);
    mPlanes[1] = BoundingPlane(0.0f, 0.0f, -1.0f, FarClip);
    mPlanes[2] = BoundingPlane(-NHx, 0.0f, -NHz, 0.0f);
    mPlanes[3] = BoundingPlane(NHx, 0.0f, -NHz, 0.0f);
    mPlanes[4] = BoundingPlane(0.0f, NVy, -NVz, 0.0f);
    mPlanes[5] = BoundingPlane(0.0f, -NVy, -NVz, 0.0f);
}