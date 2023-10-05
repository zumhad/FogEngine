#include "NormalMap.h"

#include "Utility.h"
#include "Direct3D.h"
#include "Application.h"
#include "PipelineState.h"

ID3D11RenderTargetView* NormalMap::mRenderTargetView = 0;
ID3D11ShaderResourceView* NormalMap::mShaderResourceView = 0;

void NormalMap::Setup()
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
		desc.Format = DXGI_FORMAT_R16G16B16A16_SNORM;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		FOG_TRACE(Direct3D::Device()->CreateTexture2D(&desc, 0, &texture));
	}
	{
		D3D11_RENDER_TARGET_VIEW_DESC desc{};
		desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		desc.Format = DXGI_FORMAT_R16G16B16A16_SNORM;
		FOG_TRACE(Direct3D::Device()->CreateRenderTargetView(texture, &desc, &mRenderTargetView));
	}
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc{};
		desc.Format = DXGI_FORMAT_R16G16B16A16_SNORM;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MostDetailedMip = 0;
		desc.Texture2D.MipLevels = 1;
		FOG_TRACE(Direct3D::Device()->CreateShaderResourceView(texture, &desc, &mShaderResourceView));
	}
	SAFE_RELEASE(texture);
}

void NormalMap::Shotdown()
{
	SAFE_RELEASE(mRenderTargetView);
	SAFE_RELEASE(mShaderResourceView);
}

void NormalMap::Clear()
{
	static const float color[3] = { 0.0f, 0.0f, 0.0f };
	Direct3D::DeviceContext()->ClearRenderTargetView(mRenderTargetView, color);
}

ID3D11ShaderResourceView* const* NormalMap::GetSRV()
{
	return &mShaderResourceView;
}

ID3D11RenderTargetView* NormalMap::GetRTV()
{
	return mRenderTargetView;
}