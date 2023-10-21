#pragma once

#include "Core.h"

#include "Object.h"
#include "CustomString.h"
#include "Vector3.h"
#include "Matrix.h"
#include "Texture.h"

#include <DirectXCollision.h>

class ObjectManager;
class Picking;
class DepthMap;
class SelectMap;
class PassMap;
class PipelineState;
class ShadowMap;

class FOG_API Mesh : public Object
{
	friend class ObjectManager;
	friend class Picking;
	friend class DepthMap;
	friend class SelectMap;
	friend class PassMap;
	friend class PipelineState;
	friend class ShadowMap;

public:
	TypeObject GetType() override { return TypeObject::Mesh; }

	Mesh();
	Mesh(Mesh& mesh);
	~Mesh();

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
	void Bind();
	void BindTexture();
	DirectX::BoundingBox GetBoundingBox();
	Matrix GetWorldMatrix();
	Matrix GetWorldInvTransposeMatrix();

public:
	String name;
	Material material;
	bool lighting;

	Vector3 position;
	Vector3 rotation;
	Vector3 scale;

	String texture;

private:
	struct Data;
	Data* mData;
};

