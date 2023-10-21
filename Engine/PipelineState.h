#pragma once

#include "Core.h"

#include "CustomArray.h"
#include "ConstantBuffer.h"
#include "Matrix.h"
#include "Mesh.h"
#include "Light.h"

#include <d3d11.h>

class PixelShader;
class VertexBuffer;
class IndexBuffer;
class VertexShader;
class InputLayout;
class RasterizerState;
class ScissorRect;
class RenderTargetView;
class SamplerState;
class DepthStencilState;

class PipelineState
{
public:
	static void Setup();
	static void Bind();
	static void Shotdown();

private:
	static void UpdateShadowPassViewport();
	static void UpdatePrePassViewport();
	static void UpdatePrePassBuffer(Mesh& mesh);

private:
	static RasterizerState mRasterizerState;
	static RasterizerState mShadowRasterizerState;
	static Array<ID3D11RenderTargetView*> mRenderTargetView;
	static Array<ID3D11ShaderResourceView*> mShaderResourceView;
	static SamplerState mShadowSamplerState;
	static SamplerState mSamplerState;
	static DepthStencilState mDepthStencilState;

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

	struct PrePassBuffer
	{
		Matrix worldViewProj;
		Matrix world;
		Matrix worldInvTranspose;
	};
	static ConstantBuffer<PrePassBuffer> mPrePassBuffer;
};

