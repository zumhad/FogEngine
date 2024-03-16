#pragma once

#include "Core.h"

#include "Object.h"
#include "Color.h"
#include "Vector3.h"

class FOG_API ObjectManager;
class FOG_API Model;
class FOG_API PipelineState;
class FOG_API OutlinePass;
class FOG_API PrePass;

class FOG_API DirectionLight : public Object
{
	friend class ObjectManager;
	friend class PipelineState;
	friend class OutlinePass;
	friend class PrePass;

public:
	TypeObject GetType() override { return TypeObject::DirectionLight; }

	DirectionLight();
	DirectionLight(const DirectionLight& light);
	DirectionLight(DirectionLight&& light) noexcept;
	~DirectionLight();

	Vector3 GetDirection();
	void SetColor(const Color& color);
	void SetColorR(float r);
	void SetColorG(float g);
	void SetColorB(float b);
	Color GetColor();
	void SetPower(float power);
	float GetPower();

private:
	void Bind();
	void BindTexture();
	Model* GetModel();

private:
	Color mColor;
	float mPower;

	Model* mModel;
};

class FOG_API PointLight : public Object
{
	friend class ObjectManager;
	friend class PipelineState;
	friend class OutlinePass;
	friend class PrePass;

public:
	TypeObject GetType() override { return TypeObject::PointLight; }

	PointLight();
	PointLight(const PointLight& light);
	PointLight(PointLight&& light) noexcept;
	~PointLight();

	void SetColor(const Color& color);
	void SetColorR(float r);
	void SetColorG(float g);
	void SetColorB(float b);
	Color GetColor();
	void SetRadius(float radius);
	float GetRadius();
	void SetPower(float power);
	float GetPower();

private:
	void Bind();
	void BindTexture();
	Model* GetModel();

private:
	Color mColor;
	float mRadius;
	float mPower;

	Model* mModel;
};

