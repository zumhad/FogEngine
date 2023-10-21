#pragma once

#include "Core.h"

#include <d3d11.h>

class FOG_API Application;
class FOG_API PipelineState;

class FOG_API DepthMap
{
	friend class Application;
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