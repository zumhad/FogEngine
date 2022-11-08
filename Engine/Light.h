#pragma once

#include "Core.h"

#include "Object.h"


class FOG_API DirectionalLight : public Object
{
public:
	TypeObject GetType() override { return TypeObject::DirectionalLight; }

public:
	Vector4 ambient = { 0,0,0,0 };
	Vector4 diffuse = { 1,1,1,1 };
	Vector4 specular = { 1,1,1,50 };
	Vector3 direction = { 0,-1,0 };

private:
	static int mCount;
};

class FOG_API PointLight : public Object
{
public:
	TypeObject GetType() override { return TypeObject::PointLight; }

public:
	Vector4 ambient = { 0.0f ,0.0f, 0.0f, 1.0f };
	Vector4 diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	Vector4 specular = { 0.0f, 0.0f, 0.0f, 0.0f };

	Vector3 position = { 0.0f, 0.0f, 0.0f };
	float range = 50.0f;
	Vector3 att = { 0.0f, 0.3f, 0.0f };

private:
	static int mCount;
};

