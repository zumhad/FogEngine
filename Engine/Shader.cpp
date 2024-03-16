#include "Shader.h"

#include "PathHelper.h"
#include "CustomFile.h"
#include "CustomString.h"
#include "Utility.h"
#include "CustomArray.h"

#include <vector>
#include <d3dcompiler.h>
#include <string>

void Shader::Compile(const String& fileName, const String& entryPoint, const String& shaderModel, ID3D10Blob** blob)
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

	Array<D3D_SHADER_MACRO> define;

	StringConverter temp0 = String::ToString(MAX_POINT_LIGHT);
	define.Add({ "MAX_POINT_LIGHT", temp0 });

	StringConverter temp1 = String::ToString(MAX_CASCADES);
	define.Add({ "MAX_CASCADES", temp1 });

	define.Add({ 0, 0 });

	ID3DBlob* pErrorBlob = 0;
	FOG_TRACE(D3DCompileFromFile(fileName, define.Data(), &include, StringConverter(entryPoint), StringConverter(shaderModel), dwShaderFlags, 0, blob, &pErrorBlob));

	if (pErrorBlob)
	{
		StringConverter str = (const CHAR*)pErrorBlob->GetBufferPointer();
		FOG_ERROR(String(str));
	}

	SAFE_RELEASE(pErrorBlob)
}

SHADERAPI ShaderInclude::Open(D3D_INCLUDE_TYPE, LPCSTR pFileName, LPCVOID, LPCVOID* ppData, UINT* pBytes)
{
	String includePath = PathHelper::GetAssetsPath();
	includePath += StringConverter(pFileName);

	if (File::Exists(includePath) == false)
		return E_FAIL;

	File file(includePath, File::Mode::Read);
	*pBytes = UINT(file.Size());
	UINT* data = (UINT*)(std::malloc(*pBytes));
	file.Read(data, *pBytes);
	*ppData = data;

	return S_OK;
}

SHADERAPI ShaderInclude::Close(LPCVOID pData)
{
	std::free((void*)(pData));
	return S_OK;
}