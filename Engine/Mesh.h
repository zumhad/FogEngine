#pragma once

#include "Core.h"

#include "Vector2.h"
#include "Vector3.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "BoundingBox.h"

struct VertexMax
{
	Vector3 position;
	Vector3 normal;
	Vector2 uv;
};

struct VertexMin
{
	Vector3 position;
};

template <typename T>
class FOG_API Array;

class FOG_API Model;

class FOG_API Mesh
{
	friend class Model;

public:
	Mesh(const Array<VertexMax>& vertexMax, const Array<VertexMin>& vertexMin, const Array<unsigned int>& index, const Vector3& min, const Vector3& max);
	~Mesh();

private:
	void Bind(bool normal, bool uv);
	void BindBoundingBox();

private:
	VertexBuffer mVertexBufferMax;
	VertexBuffer mVertexBufferMin;
	IndexBuffer mIndexBuffer;
	BoundingBox mBoundingBox;
};

