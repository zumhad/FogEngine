#pragma once

#include "Core.h"

#include <d3dcommon.h>

class ShaderInclude : public ID3DInclude
{
private:
	HRESULT Open(D3D_INCLUDE_TYPE, LPCSTR pFileName, LPCVOID, LPCVOID* ppData, UINT* pBytes) override;
	HRESULT Close(LPCVOID pData) override;
};

void FOG_API CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

