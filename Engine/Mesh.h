#pragma once

#include "Core.h"

#include "Object.h"
#include "CustomString.h"
#include "Vector3.h"
#include "Matrix.h"

#include <DirectXCollision.h>

class FOG_API Mesh : public Object
{
public:
	Mesh();
	Mesh(Mesh& mesh);
	~Mesh();

	void SetPosition(Vector3 v);
	void SetRotation(Vector3 v);
	void SetScale(Vector3 v);

	Vector3 GetPosition();
	Vector3 GetRotation();
	Vector3 GetScale();

	void Move(Vector3 v);
	void Rotate(Vector3 v);
	void Scale(Vector3 v);

	Vector3 GetDirection();

	void Bind();
	DirectX::BoundingBox GetBoundingBox();
	TypeObject GetType() override { return TypeObject::Mesh; }
	Matrix GetWorldMatrix();
	Matrix GetWorldInvTransposeMatrix();

public:
	String name;
	Material material;
	bool lighting;

	Vector3 position;
	Vector3 rotation;
	Vector3 scale;

private:
	struct Data;
	Data* mData;
};

