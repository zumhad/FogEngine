#include "PixelShader.h"

#include "Utility.h"
#include "Shader.h"
#include "PathHelper.h"
#include "Direct3D.h"

PixelShader::PixelShader()
{
	mPixelShader = 0;
}

void PixelShader::Release()
{
	SAFE_RELEASE(mPixelShader);
}

void PixelShader::Create(const String& name, const String& entry)
{
	SAFE_RELEASE(mPixelShader);

	String path = PathHelper::GetAssetsPath();
	path += name;

	ID3D10Blob* blob = 0;

	Shader::Compile(path, entry, L"ps_5_0", &blob);
	FOG_TRACE(Direct3D::Device()->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), 0, &mPixelShader));
}

ID3D11PixelShader* PixelShader::Get()
{
	return mPixelShader;
}