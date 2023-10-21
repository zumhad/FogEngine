#pragma once

#include "Core.h"

#include "Matrix.h"

#include <DirectXCollision.h>

class ObjectManager;
class Application;
class Camera;
class DepthMap;

class FrustumCulling
{
	friend class ObjectManager;
	friend class Application;
	friend class Camera;	
	friend class DepthMap;

private:
	static Matrix GetCascade();
	static void Setup();
	static void Shotdown();
	static void Update(Matrix view, Matrix proj);
	static bool Intersect(const DirectX::BoundingBox& bb);

private:
	struct Data;
	static Data* mData;
};