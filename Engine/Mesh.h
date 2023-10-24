#pragma once

#include "Core.h"

#include "Vector2.h"
#include "Vector3.h"

#include <d3d11.h>

struct Vertex
{
	Vector3 position;
	Vector3 normal;
	Vector2 uv;
};

template <typename T>
class FOG_API Array;

class FOG_API Model;

class FOG_API Mesh
{
	friend class Model;

public:
	Mesh(Array<Vertex>& vertex, Array<unsigned int>& index);
	~Mesh();

private:
	void Draw();

private:
	int mIndexCount;
	int mVertexSize;
	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;
};

