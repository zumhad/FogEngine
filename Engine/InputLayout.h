#pragma once

#pragma warning(push)
#pragma warning(disable : 4251)

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

	void Add(const String& name, int index = 0);
	void Create(ID3D10Blob* blob);
	void Release();
	ID3D11InputLayout* Get();


private:
	ID3D11InputLayout* mInputLayout;
	Array<D3D11_INPUT_ELEMENT_DESC> mArr;
};

#pragma warning(pop)