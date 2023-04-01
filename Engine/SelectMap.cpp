#pragma warning(disable : 6387)

#include "SelectMap.h"

#include "Trace.h"
#include "Direct3D.h"
#include "Application.h"
#include "ObjectManager.h"
#include "Camera.h"
#include "Shader.h"
#include "PathHelper.h"

ID3D11RenderTargetView* SelectMap::mRenderTargetView = 0;
ID3D11ShaderResourceView* SelectMap::mShaderResourceView = 0;

ConstantBuffer<SelectMap::SelectBuffer> SelectMap::mSelectBuffer;

bool SelectMap::mEnable = false;
bool SelectMap::mDraw = false;

void SelectMap::Setup()
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
		desc.Format = DXGI_FORMAT_R32G32B32A32_TYPELESS;
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

void SelectMap::Bind(Mesh& mesh)
{
	if (!mEnable) return;

	static SelectBuffer buffer{};
	buffer.id = mesh.GetID();

	mSelectBuffer.Bind(buffer);
}

void SelectMap::BindSRV()
{
	if (!mEnable) return;

	static FLOAT color[4]{0, 0, 0, -1};
	Direct3D::DeviceContext()->ClearRenderTargetView(mRenderTargetView, color);

	Direct3D::DeviceContext()->PSSetConstantBuffers(1, 1, mSelectBuffer.Get());
}

void SelectMap::BindRTV()
{
	if (!mDraw) return;

	Direct3D::DeviceContext()->PSSetShaderResources(1, 1, &mShaderResourceView);
}

void SelectMap::UnbindRTV()
{
	if (!mDraw) return;

	Direct3D::DeviceContext()->PSSetShaderResources(1, 1, Direct3D::NullSRV());
}

void SelectMap::Shotdown()
{
	SAFE_RELEASE(mRenderTargetView);
	SAFE_RELEASE(mShaderResourceView);

	mSelectBuffer.Release();
}

void SelectMap::SetEnable(bool enable)
{
	mEnable = enable;
}

bool SelectMap::GetEnable()
{
	return mEnable;
}

void SelectMap::SetDraw(bool draw)
{
	mDraw = draw;
}

bool SelectMap::GetDraw()
{
	return mDraw;
}

ID3D11ShaderResourceView* SelectMap::GetSRV()
{
	if (!mEnable) return 0;

	return mShaderResourceView;
}

ID3D11RenderTargetView* SelectMap::GetRTV()
{
	if (!mEnable) return 0;
	
	return mRenderTargetView;
}