#include "Raycasting.h"

using namespace DirectX;

Vector3 RayCasting::RayCast(Vector3 dir, Vector3 orig, Vector4 plane)
{
	return XMPlaneIntersectLine(plane, orig, orig + dir);
}