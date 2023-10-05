#pragma once

#include "Core.h"

#include "ConstantBuffer.h"
#include "CustomString.h"
#include "Mesh.h"

#include <d3d11.h>

class Direct3D;
class Application;
class ObjectManager;
class BufferManager;
class PipelineState;

class FOG_API DepthMap
{
	friend class Direct3D;
	friend class Application;
	friend class ObjectManager;
	friend class BufferManager;
	friend class PipelineState;

private:
	static void Setup();
	static void Shotdown();

	static void Clear();
	static void BindSRV(int id);
	static void UnbindSRV(int id);

	static ID3D11ShaderResourceView* const* GetSRV();
	static ID3D11DepthStencilView* GetDSV();

private:
	static ID3D11DepthStencilView* mDepthStencilView;
	static ID3D11ShaderResourceView* mShaderResourceView;
};