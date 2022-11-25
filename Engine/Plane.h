#pragma once


#include "Core.h"
#include "Object.h"
#include "Texture.h"

#include <DirectXCollision.h>

class FOG_API Plane : public Object
{
public:
	TypeObject GetType() override { return TypeObject::Plane; }
	DirectX::BoundingBox& GetBoundingBox() { return mBB; }

	Plane() {};
	Plane(Plane& cube);
	~Plane();

	void Bind();

	DirectX::XMMATRIX GetWorldMatrix();
	DirectX::XMMATRIX GetWorldInvTransposeMatrix();

public:
	Material material;
	bool lighting = true;

	DirectX::XMFLOAT3 scale = { 1,1,1 };
	DirectX::XMFLOAT3 position = { 0,0,0 };
	DirectX::XMFLOAT3 rotation = { 0,0,0 };

private:
	DirectX::XMFLOAT4X4 mWorld;
	DirectX::XMFLOAT4X4 mWorldInvTranspose;

	DirectX::BoundingBox mBB;
	Texture* mTexture = 0;
	ID3D11Buffer* mVertexBuffer = 0;
	ID3D11Buffer* mIndexBuffer = 0;
};


