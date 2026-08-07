#pragma once

#include "Core.h"

#include <d3d11.h>

class FOG_API PipelineState;

class FOG_API ScissorRect
{
	friend class PipelineState;

private:
	void Update(int left, int top, int right, int bottom);
	D3D11_RECT* Get();

private:
	D3D11_RECT mRect;
};

