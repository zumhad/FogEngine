#include "VertexShader.h"

#include "Shader.h"
#include "Utility.h"
#include "PathHelper.h"
#include "Direct3D.h"

VertexShader::VertexShader()
{
	mVertexShader = 0;
	mBlob = 0;
}

void VertexShader::Create(String name)
{
	SAFE_RELEASE(mVertexShader);
	SAFE_RELEASE(mBlob);

	String path = PathHelper::GetAssetsPath();
	path += name;

	Shader::Compile(path, L"VS", L"vs_5_0", &mBlob);
	FOG_TRACE(Direct3D::Device()->CreateVertexShader(mBlob->GetBufferPointer(), mBlob->GetBufferSize(), 0, &mVertexShader));
}

ID3D11VertexShader* VertexShader::Get()
{
	return mVertexShader;
}

ID3D10Blob* VertexShader::GetBlob()
{
	return mBlob;
}

void VertexShader::Release()
{
	SAFE_RELEASE(mVertexShader);
	SAFE_RELEASE(mBlob);
}