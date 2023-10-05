#pragma once

#include "Core.h"

#include "Object.h"
#include "Color.h"
#include "Vector3.h"

class FOG_API DirectionalLight : public Object
{
public:
	DirectionalLight();
	TypeObject GetType() override { return TypeObject::DirectionalLight; }

public:
	Color color;
	Vector3 direction;
	float power;

private:
	static int mCount;
};

class FOG_API PointLight : public Object
{
public:
	PointLight();
	TypeObject GetType() override { return TypeObject::PointLight; }

public:
	Color color;
	Vector3 position;
	float radius;
	float power;
	float gloss;

private:
	static int mCount;
};

