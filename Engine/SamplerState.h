#pragma once

#include "Core.h"

#include <d3d11.h>

class FOG_API PipelineState;
class FOG_API GUI;
class FOG_API OutlineMap;

class FOG_API SamplerState
{
	friend class PipelineState;
	friend class GUI;
	friend class OutlineMap;

private:
	enum class Type
	{
		Default,
		Shadow,
		GUI,
		PostProcess,
		Outline
	};

	SamplerState();

	void Create(Type type = Type::Default);
	void Release();
	ID3D11SamplerState* const* Get();

private:
	ID3D11SamplerState* mSamplerState;
};

