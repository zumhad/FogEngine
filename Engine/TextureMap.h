#pragma once

#include "Core.h"

#include "CustomString.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include <d3d11.h>

class ObjectManager;
class Application;
class Direct3D;
class PipelineState;

class FOG_API TextureMap
{
	friend class ObjectManager;
	friend class Application;
	friend class Direct3D;
	friend class PipelineState;

private:
	static void Setup();
	static void Shotdown();
	static void Bind();

private:
	static VertexBuffer mVertexBuffer;
	static IndexBuffer mIndexBuffer;
};

