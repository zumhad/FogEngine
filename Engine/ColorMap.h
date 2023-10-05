#pragma once

#include "Core.h"

#include "ConstantBuffer.h"
#include "Color.h"

class Application;
class PipelineState;
class Mesh;

class FOG_API ColorMap
{
	friend class Application;
	friend class PipelineState;

private:
	static void Setup();
	static void Shotdown();

	static void UpdateBuffer(Mesh& mesh);
	static void Clear();

	static ID3D11ShaderResourceView* const* GetSRV();
	static ID3D11RenderTargetView* GetRTV();
	static ID3D11Buffer* const* GetBuffer();

private:
	static ID3D11RenderTargetView* mRenderTargetView;
	static ID3D11ShaderResourceView* mShaderResourceView;

	struct ColorBuffer
	{
		Color color;
	};
	static ConstantBuffer<ColorBuffer> mColorBuffer;
};

