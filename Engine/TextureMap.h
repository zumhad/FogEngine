#pragma once

#include "Core.h"

#include "CustomString.h"

#include <d3d11.h>

class ObjectManager;
class Application;

class FOG_API TextureMap
{
	friend class ObjectManager;
	friend class Application;

private:
	static void Setup();
	static void Shotdown();
	static void Bind();

private:
	static ID3D11Buffer* mVertexBuffer;
	static ID3D11Buffer* mIndexBuffer;
};

