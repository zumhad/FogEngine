#pragma once

#include "Core.h"

#include "Object.h"
#include "Color.h"
#include "Vector3.h"

class FOG_API ObjectManager;
class FOG_API Model;
class FOG_API PipelineState;
class FOG_API OutlineMap;
class FOG_API PrePass;

class FOG_API DirectionLight : public Object
{
	friend class ObjectManager;
	friend class PipelineState;
	friend class OutlineMap;
	friend class PrePass;

public:
	TypeObject GetType() override { return TypeObject::DirectionLight; }

	DirectionLight();
	DirectionLight(const DirectionLight& light);
	DirectionLight(DirectionLight&& light) noexcept;
	~DirectionLight();

	Vector3 GetDirection();

private:
	void Bind();
	void BindTexture();
	Model* GetModel();

public:
	Color color;
	float power;

private:
	Model* mModel;
};

class FOG_API PointLight : public Object
{
	friend class ObjectManager;
	friend class PipelineState;
	friend class OutlineMap;
	friend class PrePass;

public:
	TypeObject GetType() override { return TypeObject::PointLight; }

	PointLight();
	PointLight(const PointLight& light);
	PointLight(PointLight&& light) noexcept;
	~PointLight();

private:
	void Bind();
	void BindTexture();
	Model* GetModel();

public:
	Color color;
	float radius;
	float power;

private:
	Model* mModel;
};

