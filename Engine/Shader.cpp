#include "Shader.h"

#include "PathHelper.h"
#include "CustomFile.h"
#include "CustomString.h"
#include "SelectMap.h"
#include "DepthMap.h"

#include <d3dcompiler.h>
#include <cstdlib>
#include <vector>
#include <string>
#include <locale>
#include <codecvt>

using namespace std;

void CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_WARNINGS_ARE_ERRORS;

#ifdef _DEBUG
	dwShaderFlags |= D3DCOMPILE_DEBUG;
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
	dwShaderFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL0;
#else
	dwShaderFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif

	ShaderInclude include;

	std::vector<D3D_SHADER_MACRO> vDefine;
	if (DepthMap::GetEnable()) vDefine.push_back({ "DEPTH_MAP", 0 });
	if (SelectMap::GetEnable()) vDefine.push_back({ "SELECT_MAP", 0 });

	if (DepthMap::GetDraw()) vDefine.push_back({ "DEPTH_MAP_DRAW", 0 });
	if (SelectMap::GetDraw()) vDefine.push_back({ "SELECT_MAP_DRAW", 0 });
	vDefine.push_back({ 0, 0 });

	ID3DBlob* pErrorBlob = 0;
	D3DCompileFromFile(szFileName, vDefine.data(), &include, szEntryPoint, szShaderModel, dwShaderFlags, 0, ppBlobOut, &pErrorBlob);

	if (pErrorBlob)
	{
		string str = (const char*)pErrorBlob->GetBufferPointer();

		int len = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, 0, 0);
		wstring out(len, 0);
		MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &out[0], len);

		FOG_ERROR(out.c_str());
	}

	SAFE_RELEASE(pErrorBlob)
}

SHADERAPI ShaderInclude::Open(D3D_INCLUDE_TYPE, LPCSTR pFileName, LPCVOID, LPCVOID* ppData, UINT* pBytes)
{
	String includePath;
	PathHelper::GetAssetsPath(includePath);
	includePath += String(pFileName);

	if (File::Exists(includePath) == false)
		return E_FAIL;

	File file(includePath, FileOpenMode::Read);
	*pBytes = UINT(file.Size());
	UINT* data = reinterpret_cast<UINT*>(std::malloc(*pBytes));
	file.Read(data, *pBytes);
	*ppData = data;

	return S_OK;
}

SHADERAPI ShaderInclude::Close(LPCVOID pData)
{
	std::free(const_cast<void*>(pData));
	return S_OK;
}