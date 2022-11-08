#pragma once

#include "Core.h"
#include "MathHelper.h"

using namespace Math;


class FOG_API BoundingPlane
{
public:
	BoundingPlane() {}
	BoundingPlane(Vector3 normalToPlane, float distanceFromOrigin) : mRepr(normalToPlane, distanceFromOrigin) {}
	BoundingPlane(float A, float B, float C, float D) : mRepr(A, B, C, D) {}

	Vector3 GetNormal() const { return Vector3(mRepr); }

	Scalar DistanceFromPoint(Vector3 point) const;

	friend BoundingPlane operator* (const OrthogonalTransform& xform, BoundingPlane plane);

private:
	Vector4 mRepr;
};