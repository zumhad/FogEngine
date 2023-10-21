#pragma once

#include "Core.h"

#include "CustomString.h"

#include <d3d11.h>

template <typename T>
class Array;

class FOG_API PipelineState;
class FOG_API GUI;

class FOG_API InputLayout
{
	friend class PipelineState;
	friend class GUI;

private:
	InputLayout();

	void Add(String name, DXGI_FORMAT format);
	void Create(ID3D10Blob* blob, Array<String>& name);
	void Release();
	ID3D11InputLayout* Get();


private:
	ID3D11InputLayout* mInputLayout;
};


