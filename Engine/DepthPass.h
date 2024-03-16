#pragma once

#include "Core.h"

#include <d3d11.h>

class FOG_API PipelineState;
class FOG_API Application;

class FOG_API DepthPass
{
	friend class PipelineState;
	friend class Application;

private:
	//static void Setup();
	//static void Shotdown();
	//static void Bind();

private:
	//static ID3D11DepthStencilView* const GetDepthDSV();
	//static ID3D11ShaderResourceView* const* GetDepthSRV();

	//static void UpdateBuffer0();

private:
	static ID3D11DepthStencilView* mDepthDSV;
	static ID3D11ShaderResourceView* mDepthSRV;
};

