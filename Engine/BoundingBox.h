#pragma once

#include "Core.h"

#include "Vector3.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class FOG_API Mesh;

class FOG_API BoundingBox
{
	friend class Mesh;

private:
	void Bind();
	void Create(const Vector3& min, const Vector3& max);
	void Release();
	Vector3 GetCenter();

private:
	Vector3 mMin;
	Vector3 mMax;

	VertexBuffer mVertexBuffer;
	IndexBuffer mIndexBuffer;
};

