#pragma once

#include "Core.h"
#include "MathHelper.h"

enum class TypeObject
{
	Cube,
	Object,
	DirectionalLight,
	PointLight,
	Plane
};


struct Vertex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 tex;
};

struct Material
{
	DirectX::XMFLOAT4 ambient = { 1.0f, 1.0f, 1.0f, 1.0f };
	DirectX::XMFLOAT4 diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	DirectX::XMFLOAT4 specular = { 0.0f, 0.0f, 0.0f, 1.0f };
	DirectX::XMFLOAT4 reflect = { 1.0f, 1.0f, 1.0f, 1.0f };
};

class FOG_API Object
{
public:
	virtual TypeObject GetType() { return TypeObject::Object; }
};
