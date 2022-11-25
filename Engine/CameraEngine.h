#pragma once

#include "Core.h"
#include "Camera.h"

class CameraEngine : public Camera
{
public:
	static bool AABB(const DirectX::BoundingBox& bb) { return mFrustum.Intersects(bb); }
	static DirectX::XMMATRIX GetViewMatrix() { return DirectX::XMLoadFloat4x4(&mView); }
	static DirectX::XMMATRIX GetProjMatrix() { return DirectX::XMLoadFloat4x4(&mProj); }
	static void Setup();
	static void Update(float dt);
	static void Set3D(bool var);

private:
	static void Update();
};

