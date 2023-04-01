#pragma once

#include "Core.h"

#include <d3dcommon.h>

#define SHADERAPI __declspec(nothrow) HRESULT __stdcall

class ShaderInclude : public ID3DInclude
{
private:
    SHADERAPI Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID* ppData, UINT* pBytes);
    SHADERAPI Close(LPCVOID pData);
};

void FOG_API CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);