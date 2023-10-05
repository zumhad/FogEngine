#pragma once

#include "Core.h"

#include <d3d11.h>

class PipelineState;

class FOG_API SamplerState
{
	friend class PipelineState;

private:
	SamplerState();

	void Create();
	void Release();
	ID3D11SamplerState* const* Get();

private:
	ID3D11SamplerState* mSamplerState;
};

