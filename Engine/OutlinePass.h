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

class FOG_API OutlinePass
{
	friend class PipelineState; 
	friend class Application;

public:
	static void Setup();
	static void Shotdown();
	static void Bind();
	static void SetColor(Color color);
	static void SetWidth(int width);

private:
	static void Clear();
	static void UpdateBuffer0();
	static void UpdateBuffer1(Model* obj);
	static void UpdateBuffer2(int stepSize);
	static void UpdateBuffer3();

private:
	static ID3D11RenderTargetView* const* GetOffsetRTV();
	static ID3D11ShaderResourceView* const* GetOffsetSRV();
	static ID3D11UnorderedAccessView* const* GetOffsetUAV();

private:
	static struct
	{
		int width;
		Color color;
	} mOutline;

	static ID3D11RenderTargetView* mOffsetRTV;
	static ID3D11ShaderResourceView* mOffsetSRV;
	static ID3D11UnorderedAccessView* mOffsetUAV;

	static VertexShader mVertexShader0;
	static PixelShader mPixelShader0;
	static InputLayout mInputLayout0;

	static ComputeShader mCopmuteShader;

	static VertexShader mVertexShader1;
	static PixelShader mPixelShader1;
	static InputLayout mInputLayout1;

	struct Buffer0;
	static ConstantBuffer<Buffer0> mBuffer0;

	struct Buffer1;
	static ConstantBuffer<Buffer1> mBuffer1;

	struct Buffer2;
	static ConstantBuffer<Buffer2> mBuffer2;

	struct Buffer3;
	static ConstantBuffer<Buffer3> mBuffer3;
};

