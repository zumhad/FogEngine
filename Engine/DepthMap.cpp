#include "DepthMap.h"

#include "ObjectManager.h"
#include "Application.h"
#include "Direct3D.h"
#include "Camera.h"
#include "Shader.h"
#include "PathHelper.h"
#include "Utility.h"

using namespace DirectX;

ID3D11DepthStencilView* DepthMap::mDepthStencilView = 0;
ID3D11ShaderResourceView* DepthMap::mShaderResourceView = 0;

void DepthMap::Setup()
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
		desc.Format = DXGI_FORMAT_R32_TYPELESS;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		FOG_TRACE(Direct3D::Device()->CreateTexture2D(&desc, 0, &texture));
	}
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC desc{};
		desc.Format = DXGI_FORMAT_D32_FLOAT;
		desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		FOG_TRACE(Direct3D::Device()->CreateDepthStencilView(texture, &desc, &mDepthStencilView));
	}
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc{};
		desc.Format = DXGI_FORMAT_R32_FLOAT;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MostDetailedMip = 0;
		desc.Texture2D.MipLevels = 1;
		FOG_TRACE(Direct3D::Device()->CreateShaderResourceView(texture, &desc, &mShaderResourceView));
	}
	SAFE_RELEASE(texture);
}

void DepthMap::Shotdown()
{
	SAFE_RELEASE(mDepthStencilView);
	SAFE_RELEASE(mShaderResourceView);
}

void DepthMap::Clear()
{
	Direct3D::DeviceContext()->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH, 0.0f, 0);
}

void DepthMap::BindSRV(int id)
{
	Direct3D::DeviceContext()->PSSetShaderResources(id, 1, &mShaderResourceView);
}

void DepthMap::UnbindSRV(int id)
{
	Direct3D::DeviceContext()->PSSetShaderResources(id, 1, Direct3D::NullSRV());
}

ID3D11ShaderResourceView* const* DepthMap::GetSRV()
{
	return &mShaderResourceView;
}

ID3D11DepthStencilView* DepthMap::GetDSV()
{
	return mDepthStencilView;
}