#pragma once

#include "Core.h"

#include <d3d11.h>

template <typename T>
class FOG_API Array;

template <typename T>
class FOG_API ConstantBuffer;

class FOG_API PixelShader;
class FOG_API VertexBuffer;
class FOG_API IndexBuffer;
class FOG_API VertexShader;
class FOG_API InputLayout;
class FOG_API RasterizerState;
class FOG_API ScissorRect;
class FOG_API RenderTargetView;
class FOG_API SamplerState;
class FOG_API DepthStencilState;
class FOG_API Model;

class FOG_API PipelineState
{
public:
	static void Setup();
	static void Bind();
	static void Shotdown();

private:
	static void UpdateShadowPassViewport();
	static void UpdatePrePassViewport();
	static void UpdatePrePassBuffer();
	static void UpdatePrePassBuffer1(Model& model);

private:
	static RasterizerState mRasterizerState;
	static RasterizerState mShadowRasterizerState;
	static RasterizerState mSkyboxRasterizerState;
	static Array<ID3D11RenderTargetView*> mRenderTargetView;
	static Array<ID3D11ShaderResourceView*> mShaderResourceView;
	static SamplerState mShadowSamplerState;
	static SamplerState mSamplerState;
	static SamplerState mPostProcessSamplerState;
	static DepthStencilState mDepthStencilState;
	static DepthStencilState mSkyboxDepthStencilState;

	static VertexShader mPrePassVS;
	static PixelShader mPrePassPS;
	static InputLayout mPrePassIL;

	static VertexShader mPassVS;
	static PixelShader mPassPS;
	static InputLayout mPassIL;

	static VertexShader mPostProcessVS;
	static PixelShader mPostProcessPS;
	static InputLayout mPostProcessIL;

	static VertexShader mShadowPassVS;
	static PixelShader mShadowPassPS;
	static InputLayout mShadowPassIL;

	struct PrePassBuffer;
	static ConstantBuffer<PrePassBuffer> mPrePassBuffer;

	struct PrePassBuffer1;
	static ConstantBuffer<PrePassBuffer1> mPrePassBuffer1;
};

