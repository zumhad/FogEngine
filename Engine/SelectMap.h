#pragma once

#include "Core.h"

#include "ConstantBuffer.h"
#include "Mesh.h"

#include <d3d11.h>

class Direct3D;
class Application;
class ObjectManager;
class BufferManager;
class Picking;
class PipelineState;

class FOG_API SelectMap
{
	friend class Direct3D;
	friend class Application;
	friend class ObjectManager;
	friend class BufferManager;
	friend class Picking;
	friend class PipelineState;

private:
	static void Setup();
	static void Shotdown();

	static void Clear();
	static void UpdateBuffer(Mesh& mesh);

	static ID3D11ShaderResourceView* const* GetSRV();
	static ID3D11RenderTargetView* GetRTV();
	static ID3D11Buffer* const* GetBuffer();

private:
	static ID3D11RenderTargetView* mRenderTargetView;
	static ID3D11ShaderResourceView* mShaderResourceView;

	struct SelectBuffer
	{
		unsigned int id; float pad[3];
	};

	static ConstantBuffer<SelectBuffer> mSelectBuffer;
};

