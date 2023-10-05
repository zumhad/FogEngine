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
	struct LightBuffer;

public:
	static void Setup();
	static void Bind();
	static void Shotdown();

private:
	static void UpdatePrePassViewport();
	static void UpdatePassViewport();
	static void UpdatePrePassBuffer(Mesh& mesh);
	static void UpdatePassBuffer();
	static void UpdateLightBuffer(LightBuffer& buffer);

private:
	static RasterizerState mRasterizerState;
	static Array<ID3D11RenderTargetView*> mRenderTargetView;
	static Array<ID3D11ShaderResourceView*> mShaderResourceView;
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

	struct PrePassBuffer
	{
		Matrix worldViewProj;
		Matrix world;
		Matrix worldInvTranspose;
	};
	static ConstantBuffer<PrePassBuffer> mPrePassBuffer;

	struct PassBuffer
	{
		int width;
		int height;
		float pad[2];
	};
	static ConstantBuffer<PassBuffer> mPassBuffer;

	struct LightBuffer
	{
		struct
		{
			Color color;
			Vector3 dir;
			float power;
		} dirLight[16];
		struct
		{
			Color color;
			Vector3 position;
			float radius;
			float power; float pad[3];
		} pointLight[16];
		Vector3 cameraPos;
		int dirCount;
		int pointCount; float pad[3];
	};
	static ConstantBuffer<LightBuffer> mLightBuffer;
};

