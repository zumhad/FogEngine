#pragma once

#include "Core.h"

#include "CustomString.h"

#include <d3d11.h>

class FOG_API OutlineMap;

class FOG_API ComputeShader
{
	friend class OutlineMap;

private:
	ComputeShader();

	void Create(String name);
	ID3D11ComputeShader* Get();
	void Release();

private:
	ID3D11ComputeShader* mComputeShader;
	ID3D10Blob* mBlob;
};

