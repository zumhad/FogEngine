#pragma once

#include "Core.h"

#include "Vector3.h"

enum class TypeObject
{
	DirectionLight,
	PointLight,
	Model
};

class FOG_API ObjectManager;

class FOG_API Object
{
	friend class ObjectManager;

public:
	virtual TypeObject GetType() = 0;
	virtual ~Object() {};

	Object();
	Object(const Object& o);

	int GetID();

public:
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;

	int data;

protected:
	int mID;
};