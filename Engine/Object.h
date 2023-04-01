#pragma once

#include "Core.h"

#include "Vector3.h"

#include <d3d11.h>
#include <DirectXMath.h>

#undef GetObject

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

class ObjectManager;

class FOG_API Object
{
	friend class ObjectManager;

public:
	Object();
	virtual TypeObject GetType() { return TypeObject::Object; }

	int GetID();

protected:
	int mID;
};