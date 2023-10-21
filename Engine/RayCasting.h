#pragma once

#include "Core.h"

class FOG_API Vector3;
class FOG_API Vector4;

class FOG_API RayCasting
{
public:
	static Vector3 RayCast(Vector3 dir, Vector3 orig, Vector4 plane);
};

