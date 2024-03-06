#pragma once

#include "Core.h"

#include <d3d11.h>

#include "Color.h"

template <typename T>
class FOG_API ConstantBuffer;

class FOG_API PipelineState;
class FOG_API Application;
class FOG_API SamplerState;
class FOG_API VertexShader;
class FOG_API PixelShader;
class FOG_API InputLayout;
class FOG_API Model;
class FOG_API ComputeShader;

class FOG_API OutlineMap
{
	friend class PipelineState; 
	friend class Application;

public:
	static void Setup();
	static void Shotdown();
	static void Clear();
	static void Bind();

	static ID3D11RenderTargetView* GetRTV();

private:
	static void UpdateInitBuffer0();
	static void UpdateInitBuffer1(Model* obj);
	static void UpdatePrePassBuffer(int stepSize);
	static void UpdatePassBuffer();

private:
	static struct
	{
		int width;
		Color color;
	} mOutline;

	static ID3D11RenderTargetView* mRenderTargetView;
	static ID3D11ShaderResourceView* mShaderResourceView;
	static ID3D11UnorderedAccessView* mUnorderedAccessView;

	static VertexShader mInitVS;
	static PixelShader mInitPS;
	static InputLayout mInitIL;
	static ComputeShader mPrePassCS;
	static VertexShader mPassVS;
	static PixelShader mPassPS;
	static InputLayout mPassIL;

	struct OutlineInitBuffer0;
	static ConstantBuffer<OutlineInitBuffer0> mOutlineInitBuffer0;

	struct OutlineInitBuffer1;
	static ConstantBuffer<OutlineInitBuffer1> mOutlineInitBuffer1;

	struct OutlinePrePassBuffer;
	static ConstantBuffer<OutlinePrePassBuffer> mOutlinePrePassBuffer;

	struct OutlinePassBuffer;
	static ConstantBuffer<OutlinePassBuffer> mOutlinePassBuffer;
};

