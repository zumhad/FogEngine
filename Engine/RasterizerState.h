#pragma once

#include "Core.h"

#include <d3d11.h>

class FOG_API PipelineState;

class FOG_API RasterizerState
{
	friend class PipelineState;

private:
	enum class Type
	{
		Default,
		Shadow,
		Skybox
	};

	RasterizerState();

	void Create(Type type = Type::Default);
	ID3D11RasterizerState* Get();
	void Release();

private:
	ID3D11RasterizerState* mRasterizerState;
};

