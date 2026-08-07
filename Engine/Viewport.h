#pragma once

#include "Core.h"

#include <d3d11.h>

class FOG_API PipelineState;

class FOG_API Viewport
{
	friend class PipelineState;

private:
	void Update(int x, int y, int width, int height);
	D3D11_VIEWPORT* Get();

private:
	D3D11_VIEWPORT mViewport;
};

