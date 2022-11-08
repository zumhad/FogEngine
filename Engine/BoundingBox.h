#pragma once

#include "Core.h"
#include "MathHelper.h"

class FOG_API BoundingBox
{
public:
	BoundingBox() {}
	BoundingBox(Vector3 min, Vector3 max) { mMin = min; mMax = max; }

	Vector3 GetMin() const { return mMin; }
	Vector3 GetMax() const { return mMax; }
private:
	Vector3 mMin;
	Vector3 mMax;
};

