#pragma once

#include "Core.h"

#include "CustomString.h"

#include <d3d11.h>

class FOG_API PipelineState;
class FOG_API GUI;
class FOG_API SkyboxPass;
class FOG_API OutlinePass;
class FOG_API PrePass;
class FOG_API ShadowPass;

class FOG_API VertexShader
{
	friend class PipelineState;
	friend class GUI;
	friend class SkyboxPass;
	friend class OutlinePass;
	friend class PrePass;
	friend class ShadowPass;

private:
	VertexShader();

	void Create(const String& name, const String& entry = L"VS");
	ID3D11VertexShader* Get();
	ID3D10Blob* GetBlob();
	void Release();

private:
	ID3D11VertexShader* mVertexShader;
	ID3D10Blob* mBlob;
};

