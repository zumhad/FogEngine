#pragma once

struct DirectionalLightBuffer
{
	DirectionalLightBuffer() { ZeroMemory(this, sizeof(this)); }

	DirectX::XMFLOAT4 ambient;
	DirectX::XMFLOAT4 diffuse;
	DirectX::XMFLOAT4 specular;
	DirectX::XMFLOAT3 direction;
	float pad;
};

struct PointLightBuffer
{
	PointLightBuffer() { ZeroMemory(this, sizeof(this)); }

	DirectX::XMFLOAT4 ambient;
	DirectX::XMFLOAT4 diffuse;
	DirectX::XMFLOAT4 specular;

	DirectX::XMFLOAT3 position;
	float range;

	DirectX::XMFLOAT3 att;
	float pad;
};