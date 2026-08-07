#pragma once

#include "Core.h"

#include <d3d11.h>

template <typename T>
class FOG_API ConstantBuffer;

class FOG_API PipelineState;
class FOG_API Application;
class FOG_API Model;
class FOG_API VertexShader;
class FOG_API InputLayout;
class FOG_API PrePass;

class FOG_API DepthPass
{
	friend class PipelineState;
	friend class Application;
	friend class PrePass;

private:
	static void Setup();
	static void Shotdown();
	static void Bind();

private:
	static ID3D11DepthStencilView* const GetDepthDSV();
	static ID3D11ShaderResourceView* const* GetDepthSRV();

	static void Clear();

	static void UpdateBuffer0();
	static void UpdateBuffer1(Model* model);

private:
	static VertexShader mVertexShader;
	static InputLayout mInputLayout;

	static ID3D11DepthStencilView* mDepthDSV;
	static ID3D11ShaderResourceView* mDepthSRV;

	struct Buffer0;
	static ConstantBuffer<Buffer0> mBuffer0;

	struct Buffer1;
	static ConstantBuffer<Buffer1> mBuffer1;
};

