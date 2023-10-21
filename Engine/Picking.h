#pragma once

#include "Core.h"

#include <d3d11.h>

class FOG_API Application;
class FOG_API Object;
class FOG_API Vector3;

class FOG_API Picking
{
	friend class Application;

public:
	static void Pick();
	static Object* GetPickObject();
	static Vector3& GetPickPosition();

private:
	static void Setup();
	static void Shotdown();

private:
	static ID3D11Texture2D* mStaging;

	static Object* mPickObject;
	static Vector3 mPickPosition;
};

