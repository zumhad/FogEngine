#pragma once

#include "Core.h"

#include "Object.h"
#include "Color.h"
#include "Vector3.h"

class FOG_API ObjectManager;
class FOG_API Model;
class FOG_API PipelineState;

class FOG_API DirectionLight : public Object
{
	friend class ObjectManager;

public:
	TypeObject GetType() override { return TypeObject::DirectionLight; }

	DirectionLight();

	Vector3 GetDirection();

public:
	Color color;
	float power;
};

class FOG_API PointLight : public Object
{
	friend class ObjectManager;
	friend class PipelineState;

public:
	TypeObject GetType() override { return TypeObject::PointLight; }

	PointLight();
	PointLight(const PointLight& light);
	PointLight(PointLight&& light) noexcept;
	~PointLight();

private:
	void Bind();
	Model& GetModel();

public:
	Color color;
	float radius;
	float power;

private:
	Model* mModel;
};

