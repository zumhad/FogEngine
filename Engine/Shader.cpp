#include "Shader.h"

#include "PathHelper.h"
#include "CustomFile.h"

#include <d3dcompiler.h>

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

	ID3DBlob* pErrorBlob = 0;
	FOG_TRACE(D3DCompileFromFile(szFileName, 0, &include, szEntryPoint, szShaderModel, dwShaderFlags, 0, ppBlobOut, &pErrorBlob));

	SAFE_RELEASE(pErrorBlob);
}

HRESULT ShaderInclude::Open(D3D_INCLUDE_TYPE, LPCSTR pFileName, LPCVOID, LPCVOID* ppData, UINT* pBytes)
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

HRESULT ShaderInclude::Close(LPCVOID pData)
{
	std::free(const_cast<void*>(pData));
	return S_OK;
}