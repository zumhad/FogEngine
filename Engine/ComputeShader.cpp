#include "ComputeShader.h"

#include "Utility.h"
#include "PathHelper.h"
#include "Direct3D.h"
#include "Shader.h"

ComputeShader::ComputeShader()
{
	mComputeShader = 0;
	mBlob = 0;
}

void ComputeShader::Create(const String& name, const String& entry)
{
	String path = PathHelper::GetAssetsPath();
	path += name;

	Shader::Compile(path, entry, L"cs_5_0", &mBlob);
	FOG_TRACE(Direct3D::Device()->CreateComputeShader(mBlob->GetBufferPointer(), mBlob->GetBufferSize(), 0, &mComputeShader));
}

ID3D11ComputeShader* ComputeShader::Get()
{
	return mComputeShader;
}

void ComputeShader::Release()
{
	SAFE_RELEASE(mComputeShader);
	SAFE_RELEASE(mBlob);
}