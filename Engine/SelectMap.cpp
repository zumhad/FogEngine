#include "SelectMap.h"

#include "Direct3D.h"
#include "Application.h"
#include "ObjectManager.h"
#include "Camera.h"
#include "Shader.h"
#include "PathHelper.h"
#include "PipelineState.h"
#include "ConstantBuffer.h"
#include "Mesh.h"

ID3D11RenderTargetView* SelectMap::mRenderTargetView = 0;
ID3D11ShaderResourceView* SelectMap::mShaderResourceView = 0;

struct SelectMap::SelectBuffer
{
	unsigned int id; float pad[3];
};

ConstantBuffer<SelectMap::SelectBuffer> SelectMap::mSelectBuffer;

void SelectMap::Setup()
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

	ID3D11Texture2D* texture = 0;
	{
		D3D11_TEXTURE2D_DESC desc{};
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		FOG_TRACE(Direct3D::Device()->CreateTexture2D(&desc, 0, &texture));
	}
	{
		D3D11_RENDER_TARGET_VIEW_DESC desc{};
		desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		FOG_TRACE(Direct3D::Device()->CreateRenderTargetView(texture, &desc, &mRenderTargetView));
	}
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc{};
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MostDetailedMip = 0;
		desc.Texture2D.MipLevels = 1;
		FOG_TRACE(Direct3D::Device()->CreateShaderResourceView(texture, &desc, &mShaderResourceView));
	}
	SAFE_RELEASE(texture);

	mSelectBuffer.Create();
}

void SelectMap::Clear()
{
	static const float color[4]{ 0.0f, 0.0f, 0.0f, -1.0f };
	Direct3D::DeviceContext()->ClearRenderTargetView(mRenderTargetView, color);
}

void SelectMap::UpdateBuffer(Mesh& mesh)
{
	static SelectBuffer buffer{};
	buffer.id = mesh.GetID();

	mSelectBuffer.Bind(buffer);
}

void SelectMap::Shotdown()
{
	SAFE_RELEASE(mRenderTargetView);
	SAFE_RELEASE(mShaderResourceView);

	mSelectBuffer.Release();
}

ID3D11ShaderResourceView* const* SelectMap::GetSRV()
{
	return &mShaderResourceView;
}

ID3D11RenderTargetView* SelectMap::GetRTV()
{
	return mRenderTargetView;
}

ID3D11Buffer* const* SelectMap::GetBuffer()
{
	return mSelectBuffer.Get();
}