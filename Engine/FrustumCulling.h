#pragma once

#include "Core.h"

#include "Matrix.h"

#include <DirectXCollision.h>

class FrustumCulling
{
public:
	static void Setup();
	static void Shotdown();
	static void Update(Matrix view, Matrix proj);
	static bool Intersect(const DirectX::BoundingBox& bb);

private:
	struct Data;
	static Data* mData;
};