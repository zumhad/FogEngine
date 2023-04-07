#include "DepthMap.h"

#pragma warning(disable : 6387)

#include "ObjectManager.h"
#include "Application.h"
#include "Direct3D.h"
#include "FrustumCulling.h"
#include "Camera.h"
#include "Shader.h"
#include "PathHelper.h"

using namespace DirectX;

ID3D11DepthStencilView* DepthMap::mDepthStencilView = 0;
ID3D11ShaderResourceView* DepthMap::mShaderResourceView = 0;
ID3D11DepthStencilState* DepthMap::mDepthStencilState = 0;

bool DepthMap::mEnable = false;
bool DepthMap::mDraw = false;

void DepthMap::Setup()
{
	float width, height;

	if (Application::IsGame())
	{
		width = (float)Application::GetGameWidth();
		height = (float)Application::GetGameHeight();
	}
	else
	{
		width = (float)Application::GetSceneWidth();
		height = (float)Application::GetSceneHeight();
	}

	ID3D11Texture2D* texture = 0;
	{
		D3D11_TEXTURE2D_DESC desc{};
		desc.Width = (UINT)width;
		desc.Height = (UINT)height;
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

	{
		D3D11_DEPTH_STENCIL_DESC desc{};
		desc.DepthEnable = true;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		desc.DepthFunc = D3D11_COMPARISON_GREATER;
		FOG_TRACE(Direct3D::Device()->CreateDepthStencilState(&desc, &mDepthStencilState));
	}
}

void DepthMap::Shotdown()
{
	SAFE_RELEASE(mDepthStencilView);
	SAFE_RELEASE(mShaderResourceView);
	SAFE_RELEASE(mDepthStencilState);
}


void DepthMap::Bind(Mesh&)
{
	return;
}

void DepthMap::BindSRV()
{
	if (!mEnable) return;

	Direct3D::DeviceContext()->OMSetDepthStencilState(mDepthStencilState, 0);

	Direct3D::DeviceContext()->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH, 0.0f, 0);
}

void DepthMap::BindRTV()
{
	if (!mDraw) return;

	Direct3D::DeviceContext()->PSSetShaderResources(0, 1, &mShaderResourceView);
}

void DepthMap::UnbindRTV()
{
	if (!mDraw) return;

	Direct3D::DeviceContext()->PSSetShaderResources(0, 1, Direct3D::NullSRV());
}

ID3D11ShaderResourceView* DepthMap::GetSRV()
{
	if (!mEnable) return 0;

	return mShaderResourceView;
}

ID3D11DepthStencilView* DepthMap::GetDSV()
{
	if (!mEnable) return 0;

	return mDepthStencilView;
}

void DepthMap::SetEnable(bool enable)
{
	mEnable = enable;
}

bool DepthMap::GetEnable()
{
	return mEnable;
}

void DepthMap::SetDraw(bool draw)
{
	mDraw = draw;
}

bool DepthMap::GetDraw()
{
	return mDraw;
}