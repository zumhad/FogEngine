#pragma once

#include "Core.h"

#include <d3d11.h>

class FOG_API ObjectManager;
class FOG_API Application;
class FOG_API Direct3D;
class FOG_API PipelineState;
class FOG_API VertexBuffer;
class FOG_API IndexBuffer;
class FOG_API OutlinePass;

class FOG_API TextureMap
{
	friend class ObjectManager;
	friend class Application;
	friend class Direct3D;
	friend class PipelineState;
	friend class OutlinePass;

private:
	static void Setup();
	static void Shotdown();
	static void Bind();

private:
	static VertexBuffer mVertexBuffer;
	static IndexBuffer mIndexBuffer;
};

