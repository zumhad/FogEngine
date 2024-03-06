#pragma once

#include "Core.h"

#include "CustomString.h"

#include <d3d11.h>

class FOG_API PipelineState;
class FOG_API GUI;
class FOG_API Skybox;
class FOG_API OutlineMap;
class FOG_API PrePass;

class FOG_API VertexShader
{
	friend class PipelineState;
	friend class GUI;
	friend class Skybox;
	friend class OutlineMap;
	friend class PrePass;

private:
	VertexShader();

	void Create(String name);
	ID3D11VertexShader* Get();
	ID3D10Blob* GetBlob();
	void Release();

private:
	ID3D11VertexShader* mVertexShader;
	ID3D10Blob* mBlob;
};

