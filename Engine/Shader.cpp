#include "Shader.h"

#include "PathHelper.h"
#include "CustomFile.h"
#include "CustomString.h"
#include "SelectMap.h"
#include "DepthMap.h"
#include "ColorMap.h"

#include <d3dcompiler.h>
#include <cstdlib>
#include <vector>
#include <locale>
#include <codecvt>

using namespace std;

void Shader::Compile(String fileName, String entryPoint, String shaderModel, ID3D10Blob** blob)
{
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_WARNINGS_ARE_ERRORS;

#ifdef _DEBUG
	dwShaderFlags |= D3DCOMPILE_DEBUG;
	//dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
	dwShaderFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL0;
#else
	dwShaderFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif

	ShaderInclude include;

	std::vector<D3D_SHADER_MACRO> vDefine;
	//if (DepthMap::GetEnable()) vDefine.push_back({ "DEPTH_MAP", 0 });
	//if (SelectMap::GetEnable()) vDefine.push_back({ "SELECT_MAP", 0 });
	//if (ColorMap::GetEnable()) vDefine.push_back({ "COLOR_MAP", 0 });

	//if (DepthMap::GetDraw()) vDefine.push_back({ "DEPTH_MAP_DRAW", 0 });
	//if (SelectMap::GetDraw()) vDefine.push_back({ "SELECT_MAP_DRAW", 0 });
	//if (ColorMap::GetDraw()) vDefine.push_back({ "COLOR_MAP_DRAW", 0 });
	vDefine.push_back({ 0, 0 });

	ID3DBlob* pErrorBlob = 0;
	FOG_TRACE(D3DCompileFromFile(fileName.GetWCHAR(), vDefine.data(), &include, entryPoint.GetCHAR(), shaderModel.GetCHAR(), dwShaderFlags, 0, blob, &pErrorBlob));

	if (pErrorBlob)
	{
		String str = (const CHAR*)pErrorBlob->GetBufferPointer();

		FOG_ERROR(str);
	}

	SAFE_RELEASE(pErrorBlob)
}

SHADERAPI ShaderInclude::Open(D3D_INCLUDE_TYPE, LPCSTR pFileName, LPCVOID, LPCVOID* ppData, UINT* pBytes)
{
	String includePath = PathHelper::GetAssetsPath();
	includePath += String(pFileName);

	if (File::Exists(includePath) == false)
		return E_FAIL;

	File file(includePath, FileOpenMode::Read);
	*pBytes = UINT(file.Size());
	UINT* data = (UINT*)(std::malloc(*pBytes));
	file.Read(data, *pBytes);
	*ppData = data;

	return S_OK;
}

SHADERAPI ShaderInclude::Close(LPCVOID pData)
{
	std::free(const_cast<void*>(pData));
	return S_OK;
}