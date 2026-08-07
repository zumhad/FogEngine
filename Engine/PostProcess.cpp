#include "PostProcess.h"

#include "Application.h"
#include "ConstantBuffer.h"

struct PostProcess::PostProcessBuffer
{
	float invWidth;
	float invHeight; float pad[2];
};

ConstantBuffer<PostProcess::PostProcessBuffer> PostProcess::mPostProcessBuffer;

void PostProcess::UpdateBuffer()
{
	int width, height;

	if (Application::IsGame())
	{
		width = Application::GetGameWidth();
		height = Application::GetGameHeight();
	}
	else
	{
		width = Application::GetSceneWidth();
		height = Application::GetSceneHeight();
	}

	static PostProcessBuffer buffer;
	buffer.invWidth = 1.0f / width;
	buffer.invHeight = 1.0f / height;

	mPostProcessBuffer.Bind(buffer);
}

void PostProcess::Setup()
{
	mPostProcessBuffer.Create();
}

void PostProcess::Shotdown()
{
	mPostProcessBuffer.Release();
}

ID3D11Buffer* const* PostProcess::GetBuffer()
{
	return mPostProcessBuffer.Get();
}