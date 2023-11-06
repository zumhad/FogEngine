#pragma once

#include "Core.h"

#include <d3d11.h>

template <typename T>
class FOG_API ConstantBuffer;

class FOG_API Application;
class FOG_API PipelineState;
class FOG_API Model;

class FOG_API NormalMap
{
	friend class Application;
	friend class PipelineState;

private:
	static void Setup();
	static void Shotdown();

	static void Clear();
	static void UpdateBuffer(Model& model);

	static ID3D11ShaderResourceView* const* GetSRV();
	static ID3D11RenderTargetView* GetRTV();
	static ID3D11Buffer* const* GetBuffer();

private:
	static ID3D11RenderTargetView* mRenderTargetView;
	static ID3D11ShaderResourceView* mShaderResourceView;

	struct FOG_API NormalBuffer;
	static ConstantBuffer<NormalBuffer> mNormalBuffer;
};

