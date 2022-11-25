#pragma once

#include "Core.h"
#include "Object.h"

#include <SimpleMath.h>

class FOG_API DirectionalLight : public Object
{
public:
	TypeObject GetType() override { return TypeObject::DirectionalLight; }

public:
	DirectX::SimpleMath::Color ambient = { 0.0f, 0.0f, 0.0f, 0.0f };
	DirectX::SimpleMath::Color diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	DirectX::SimpleMath::Color specular = { 1.0f, 1.0f, 1.0f, 50.0f };
	DirectX::XMFLOAT3 direction = { 0.0f, -1.0f, 0.0f };

private:
	static int mCount;
};

class FOG_API PointLight : public Object
{
public:
	TypeObject GetType() override { return TypeObject::PointLight; }

public:
	DirectX::SimpleMath::Color ambient = { 0.0f ,0.0f, 0.0f, 1.0f };
	DirectX::SimpleMath::Color diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	DirectX::SimpleMath::Color specular = { 0.0f, 0.0f, 0.0f, 0.0f };

	DirectX::XMFLOAT3 position = { 0.0f, 0.0f, 0.0f };
	float range = 500.0f;
	DirectX::XMFLOAT3 att = { 0.0f, 0.3f, 0.0f };

private:
	static int mCount;
};

