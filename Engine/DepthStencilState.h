#pragma once

#include "Core.h"

#include <d3d11.h>

enum class DepthStencilStateType
{
	Default,
	GUI
};

class FOG_API PipelineState;
class FOG_API GUI;

class FOG_API DepthStencilState
{
	friend class PipelineState;
	friend class GUI;

private:
	DepthStencilState();

	void Create(DepthStencilStateType type = DepthStencilStateType::Default);
	void Release();
	ID3D11DepthStencilState* Get();

private:
	ID3D11DepthStencilState* mDepthStencilState;
};