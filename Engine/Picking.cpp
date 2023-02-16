#include "Picking.h"

#include <DirectXCollision.h>
#include <SimpleMath.h>

using namespace DirectX;

Vector3 Picking::Pick(Vector3 dir, Vector3 orig)
{
	XMVECTOR plane = XMVectorSet(0, 1, 0, 0);

	//SimpleMath::Plane p;
	
	return XMPlaneIntersectLine(plane, orig, orig + dir);
}