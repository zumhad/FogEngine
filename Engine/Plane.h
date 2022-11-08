#pragma once


#include "Core.h"
#include "Object.h"
#include "BoundingBox.h"
#include "Texture.h"



class FOG_API Plane : public Object
{
public:
	TypeObject GetType() override { return TypeObject::Plane; }
	BoundingBox& GetBoundingBox() { return bb; }

	Plane() {};
	Plane(Plane& cube);
	~Plane();

	void Bind();

	Matrix4& GetWorldMatrix();
	Matrix4& GetWorldInvTransposeMatrix();

public:
	Material material;
	bool lighting = true;

	Vector3 scale = Vector3(1.0f, 1.0f, 1.0f);
	Vector3 position = Vector3::Zero();
	Vector3 rotation = Vector3::Zero();

private:
	Quaternion mQRotation;
	Matrix4 mWorld = Matrix4::Identity();
	Matrix4 mWorldInvTranspose;

	BoundingBox bb;
	Texture* mTexture = 0;
	ID3D11Buffer* mVertexBuffer = 0;
	ID3D11Buffer* mIndexBuffer = 0;
};


