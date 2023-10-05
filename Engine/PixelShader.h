#pragma once

#include "Core.h"

#include "CustomString.h"

#include <d3d11.h>

class PipelineState;

class FOG_API PixelShader
{
	friend class PipelineState;

private:
	PixelShader();

	void Create(String name);
	void Release();

	ID3D11PixelShader* Get();

private:
	ID3D11PixelShader* mPixelShader;
};

