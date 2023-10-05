#pragma once

#include "Core.h"

#include <d3d11.h>

class PipelineState;

class FOG_API DepthStencilState
{
	friend class PipelineState;

private:
	DepthStencilState();

	void Create();
	void Release();
	ID3D11DepthStencilState* Get();

private:
	ID3D11DepthStencilState* mDepthStencilState;
};

