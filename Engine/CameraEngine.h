#pragma once

#include "Camera.h"

class CameraEngine : public Camera
{
public:
	static bool AABB(const BoundingBox& bb) { return mFrustumWS.IntersectBoundingBox(bb); }
	static Matrix4& GetViewMatrix() { return mView; }
	static Matrix4& GetProjMatrix() { return mProj; }
	static Matrix4& GetWorldMatrix() { return mWorld; }
	static void Setup();
	static void Update(float dt);
	static void RestartMatrix();
	static void Set3D(bool var) { m3D = var; UpdateProjMatrix(); }
};

