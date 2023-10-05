#pragma once

#include "Core.h"

#include <d3d11.h>

class PipelineState;

class FOG_API RasterizerState
{
	friend class PipelineState;

private:
	RasterizerState();

	void Create();
	ID3D11RasterizerState* Get();
	void Release();

private:
	ID3D11RasterizerState* mRasterizerState;
};

