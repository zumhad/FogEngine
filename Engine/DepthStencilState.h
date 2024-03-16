#pragma once

#include "Core.h"

#include <d3d11.h>

class FOG_API PipelineState;
class FOG_API GUI;

class FOG_API DepthStencilState
{
	friend class PipelineState;
	friend class GUI;

private:
	enum class Type
	{
		ReadOnly,
		Disable,
		ReadWrite
	};

	DepthStencilState();

	void Create(Type type = Type::ReadWrite);
	void Release();
	ID3D11DepthStencilState* Get();

private:
	ID3D11DepthStencilState* mDepthStencilState;
};