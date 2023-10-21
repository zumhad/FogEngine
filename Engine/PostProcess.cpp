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
	int x, y;
	int width, height;

	if (Application::IsGame())
	{
		x = 0;
		y = 0;
		width = Application::GetGameWidth();
		height = Application::GetGameHeight();
	}
	else
	{
		x = Application::GetSceneX();
		y = Application::GetSceneY();
		width = Application::GetSceneWidth();
		height = Application::GetSceneHeight();
	}

	static PostProcessBuffer buffer;
	buffer.invWidth = 1.0f / width;
	buffer.invHeight = 1.0f / height;

	mPostProcessBuffer.Bind(buffer);

	static D3D11_VIEWPORT viewport{};
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = (FLOAT)x;
	viewport.TopLeftY = (FLOAT)y;
	viewport.Width = (FLOAT)width;
	viewport.Height = (FLOAT)height;

	static D3D11_RECT rect{};
	rect.left = x;
	rect.top = y;
	rect.right = width + x;
	rect.bottom = height + y;

	Direct3D::DeviceContext()->RSSetViewports(1, &viewport);
	Direct3D::DeviceContext()->RSSetScissorRects(1, &rect);
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