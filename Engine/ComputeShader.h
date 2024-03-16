#pragma once

#include "Core.h"

#include "CustomString.h"

#include <d3d11.h>

class FOG_API OutlinePass;

class FOG_API ComputeShader
{
	friend class OutlinePass;

private:
	ComputeShader();

	void Create(const String& name, const String& entry = L"CS");
	ID3D11ComputeShader* Get();
	void Release();

private:
	ID3D11ComputeShader* mComputeShader;
	ID3D10Blob* mBlob;
};

