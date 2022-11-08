#pragma once

#include "Camera.h"

class CameraEngine : public Camera
{
public:
	static bool AABB(const BoundingBox& bb) { return mFrustumWS.IntersectBoundingBox(bb); }
	static Matrix4& GetViewMatrix() { return mView; }
	static Matrix4& GetProjMatrix() { return mProj; }
	static void Setup();
	static void Update(float dt);
};

