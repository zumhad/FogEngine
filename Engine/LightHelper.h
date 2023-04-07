#pragma once

#include "Core.h"

struct DirectionalLightBuffer
{
	DirectX::XMFLOAT4 color;
	DirectX::XMFLOAT3 direction; float pad;
};

struct PointLightBuffer
{
	DirectX::XMFLOAT4 color;
	DirectX::XMFLOAT3 position;
	float range;
	float power; float pad[3];
};