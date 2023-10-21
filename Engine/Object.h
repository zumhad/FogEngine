#pragma once

#include "Core.h"

enum class TypeObject
{
	DirectionalLight,
	PointLight,
	Mesh
};

class FOG_API ObjectManager;

class FOG_API Object
{
	friend class ObjectManager;

public:
	Object();
	virtual ~Object() {};
	virtual TypeObject GetType() = 0;

	int GetID();

protected:
	int mID;
};