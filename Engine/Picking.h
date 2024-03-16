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
	static void SetPickObject(Object* obj);
	static Vector3& GetPickPosition();

private:
	static void Setup();
	static void Shotdown();

	static Vector3 ReadPosition();
	static int ReadID();


private:
	static ID3D11Texture2D* mStagingPosition;
	static ID3D11Texture2D* mStagingID;

	static Object* mPickObject;
	static Vector3 mPickPosition;
};

