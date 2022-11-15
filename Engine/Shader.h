#pragma once

#include "Core.h"

#include "PathHelper.h"
#include "CustomFile.h"

#include <d3dcompiler.h>


class ShaderInclude : public ID3DInclude
{
	HRESULT Open(D3D_INCLUDE_TYPE, LPCSTR pFileName, LPCVOID, LPCVOID* ppData, UINT* pBytes) override
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

	HRESULT Close(LPCVOID pData) override
	{
		std::free(const_cast<void*>(pData));
		return S_OK;
	}
};

void FOG_API CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

