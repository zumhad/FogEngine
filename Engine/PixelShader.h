#pragma once

#include "Core.h"

#include "CustomString.h"
#include "CustomArray.h"

#include <d3d11.h>

class FOG_API PipelineState;
class FOG_API GUI;
class FOG_API Skybox;
class FOG_API OutlineMap;
class FOG_API PrePass;

class FOG_API PixelShader
{
	friend class PipelineState;
	friend class GUI;
	friend class Skybox;
	friend class OutlineMap;
	friend class PrePass;

private:
	PixelShader();

	void Create(String name);
	void Release();

	ID3D11PixelShader* Get();

private:
	ID3D11PixelShader* mPixelShader;
};

