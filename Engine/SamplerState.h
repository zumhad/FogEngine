#pragma once

#include "Core.h"

#include <d3d11.h>

enum class SamplerStateType
{
	Default,
	Shadow,
	GUI
};

class PipelineState;
class GUI;

class FOG_API SamplerState
{
	friend class PipelineState;
	friend class GUI;

private:
	SamplerState();

	void Create(SamplerStateType type = SamplerStateType::Default);
	void Release();
	ID3D11SamplerState* const* Get();

private:
	ID3D11SamplerState* mSamplerState;
};

