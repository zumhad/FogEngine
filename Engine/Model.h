#pragma once

#pragma warning(push)
#pragma warning(disable : 4251)

#include "Core.h"

#include "CustomArray.h"
#include "Color.h"
#include "CustomString.h"
#include "Matrix.h"
#include "Matrix3.h"
#include "Texture.h"
#include "Object.h"

class FOG_API PipelineState;
class FOG_API ShadowMap;
class FOG_API Mesh;
class FOG_API Skybox;
class FOG_API ObjectManager;
class FOG_API PointLight;

class FOG_API Model : public Object
{
	friend class PipelineState;
	friend class ShadowMap;
	friend class Skybox;
	friend class ObjectManager;
	friend class PointLight;

public:
	TypeObject GetType() override { return TypeObject::Model; }

	Model();
	Model(const Model& model);
	Model(Model&& model) noexcept;
	~Model();

	void SetPosition(Vector3 v);
	void SetRotation(Vector3 v);
	void SetScale(Vector3 v);

	Vector3 GetPosition();
	Vector3 GetRotation();
	Vector3 GetScale();
	Vector3 GetDirection();

	void Move(Vector3 v);
	void Rotate(Vector3 v);
	void Scale(Vector3 v);

private:
	void BindTexture();
	void Draw();

	Matrix GetWorldMatrix();
	Matrix GetWorldInvTransposeMatrix(Matrix& world);

public:
	String name;
	String texture;
	Color color;
	bool lighting;

private:
	Array<Mesh*> mArr;
	Texture mTexture;
};

#pragma warning(pop)