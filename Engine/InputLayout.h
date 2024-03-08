#pragma once

#include "Core.h"

#include "CustomString.h"

#include <d3d11.h>

template <typename T>
class Array;

class FOG_API PipelineState;
class FOG_API GUI;
class FOG_API SkyboxPass;
class FOG_API OutlinePass;
class FOG_API PrePass;
class FOG_API ShadowPass;

class FOG_API InputLayout
{
	friend class PipelineState;
	friend class GUI;
	friend class SkyboxPass;
	friend class OutlinePass;
	friend class PrePass;
	friend class ShadowPass;

private:
	InputLayout();

	void Add(String name, DXGI_FORMAT format);
	void Create(ID3D10Blob* blob, Array<String>& name);
	void Release();
	ID3D11InputLayout* Get();


private:
	ID3D11InputLayout* mInputLayout;
};


