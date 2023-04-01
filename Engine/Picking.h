#pragma once

#include "Core.h"

#include "Vector3.h"
#include "Vector4.h"
#include "Object.h"

#include <d3d11.h>

class Application;

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

