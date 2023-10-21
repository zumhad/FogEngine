#include "Raycasting.h"

#include "Vector3.h"
#include "Vector4.h"

using namespace DirectX;

Vector3 RayCasting::RayCast(Vector3 dir, Vector3 orig, Vector4 plane)
{
	return XMPlaneIntersectLine(plane, orig, orig + dir);
}