#pragma once

#include "Core.h"

#include <d3d11.h>

class Application;
class PipelineState;

class FOG_API NormalMap
{
	friend class Application;
	friend class PipelineState;

private:
	static void Setup();
	static void Shotdown();

	static void Clear();

	static ID3D11ShaderResourceView* const* GetSRV();
	static ID3D11RenderTargetView* GetRTV();

private:
	static ID3D11RenderTargetView* mRenderTargetView;
	static ID3D11ShaderResourceView* mShaderResourceView;
};

