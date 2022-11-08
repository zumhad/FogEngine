#include "BoundingPlane.h"

Scalar BoundingPlane::DistanceFromPoint(Vector3 point) const
{
	return Dot(point, GetNormal()) + mRepr.GetW();
}

BoundingPlane operator*(const OrthogonalTransform& xform, BoundingPlane plane)
{
	Vector3 normalToPlane = xform.GetRotation() * plane.GetNormal();
	float distanceFromOrigin = plane.mRepr.GetW() - Dot(normalToPlane, xform.GetTranslation());
	return BoundingPlane(normalToPlane, distanceFromOrigin);
}
