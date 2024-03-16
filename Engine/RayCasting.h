#pragma once

#include "Core.h"

class FOG_API Vector3;
class FOG_API Vector4;

class FOG_API RayCasting
{
public:
	static Vector3 RayCast(const Vector3& dir, const Vector3& orig, const Vector4& plane);
};

