#pragma once

#include "Core.h"

#include <d3d11.h>
#include <DirectXMath.h>

enum class TypeObject
{
	Object,
	DirectionalLight,
	PointLight,
	Mesh
};

struct Material
{
	DirectX::XMFLOAT4 ambient = { 1.0f, 1.0f, 1.0f, 1.0f };
	DirectX::XMFLOAT4 diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	DirectX::XMFLOAT4 specular = { 0.0f, 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT4 reflect = { 1.0f, 1.0f, 1.0f, 1.0f };
};

class FOG_API Object
{
public:
	Object();
	virtual TypeObject GetType() { return TypeObject::Object; }

protected:
	int id;
};
