#pragma once

#include "Core.h"

#include "CustomString.h"

#include <d3dcommon.h>

#define SHADERAPI __declspec(nothrow) HRESULT __stdcall

class ShaderInclude : public ID3DInclude
{
private:
    SHADERAPI Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID* ppData, UINT* pBytes);
    SHADERAPI Close(LPCVOID pData);
};

class Shader
{
public:
    static void Compile(String fileName, String entryPoint, String shaderModel, ID3D10Blob** blob);
};