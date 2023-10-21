#pragma once

#include "Core.h"

#include "Object.h"
#include "Vector3.h"
#include "Matrix.h"
#include "Texture.h"
#include "Color.h"

#include <d3d11.h>

class FOG_API ObjectManager;
class FOG_API Picking;
class FOG_API DepthMap;
class FOG_API SelectMap;
class FOG_API PassMap;
class FOG_API PipelineState;
class FOG_API ShadowMap;

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
	Matrix GetWorldMatrix();
	Matrix GetWorldInvTransposeMatrix(Matrix& world);

public:
	String name;
	Color color;
	bool lighting;

	Vector3 position;
	Vector3 rotation;
	Vector3 scale;

	String texture;

private:
	Texture mTexture;

	int mIndexCount;
	int mVertexSize;
	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;
};

