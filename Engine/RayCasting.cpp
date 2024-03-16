#include "Raycasting.h"

#include "Vector3.h"
#include "Vector4.h"

using namespace DirectX;

Vector3 RayCasting::RayCast(const Vector3& dir, const Vector3& orig, const Vector4& plane)
{
	return XMPlaneIntersectLine(plane, orig, orig + dir);
}