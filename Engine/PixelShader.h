#pragma once

#include "Core.h"

#include "CustomString.h"
#include "CustomArray.h"

#include <d3d11.h>

class FOG_API PipelineState;
class FOG_API GUI;
class FOG_API SkyboxPass;
class FOG_API OutlinePass;
class FOG_API PrePass;
class FOG_API ShadowPass;

class FOG_API PixelShader
{
	friend class PipelineState;
	friend class GUI;
	friend class SkyboxPass;
	friend class OutlinePass;
	friend class PrePass;
	friend class ShadowPass;

private:
	PixelShader();

	void Create(String name, String entry = L"PS");
	void Release();

	ID3D11PixelShader* Get();

private:
	ID3D11PixelShader* mPixelShader;
};

