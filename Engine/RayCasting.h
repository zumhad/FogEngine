#pragma once

#include "Core.h"

#include "Vector3.h"
#include "Vector4.h"

class FOG_API RayCasting
{
public:
	static Vector3 RayCast(Vector3 dir, Vector3 orig, Vector4 plane);
};

