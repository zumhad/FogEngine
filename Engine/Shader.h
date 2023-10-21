#pragma once

#include "Core.h"

#include <d3dcommon.h>

#define SHADERAPI __declspec(nothrow) HRESULT __stdcall

class FOG_API String;

class FOG_API ShaderInclude : public ID3DInclude
{
private:
    SHADERAPI Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID* ppData, UINT* pBytes);
    SHADERAPI Close(LPCVOID pData);
};

class FOG_API Shader
{
public:
    static void Compile(String fileName, String entryPoint, String shaderModel, ID3D10Blob** blob);
};