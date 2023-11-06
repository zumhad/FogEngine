#pragma warning(disable : 6387)

#include "ShadowMap.h"

#include "Application.h"
#include "Camera.h"
#include "Matrix.h"
#include "ConstantBuffer.h"
#include "Model.h"

struct ShadowMap::ShadowBuffer
{
	Matrix worldViewProj;
};

ID3D11DepthStencilView* ShadowMap::mDepthStencilView = 0;
ID3D11ShaderResourceView* ShadowMap::mShaderResourceView = 0;
ConstantBuffer<ShadowMap::ShadowBuffer> ShadowMap::mShadowBuffer;
int ShadowMap::mResolution = 1024;

void ShadowMap::Setup()
{
	UpdateTexture();

	mShadowBuffer.Create();
}

void ShadowMap::UpdateTexture()
{
	SAFE_RELEASE(mShaderResourceView);
	SAFE_RELEASE(mDepthStencilView);

	ID3D11Texture2D* texture = 0;
	{
		D3D11_TEXTURE2D_DESC desc{};
		desc.Width = mResolution;
		desc.Height = mResolution;
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

void ShadowMap::Clear()
{
	Direct3D::DeviceContext()->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH, 0.0f, 0);
}

void ShadowMap::UpdateBuffer(Model& model)
{
	static ShadowBuffer buffer;

	buffer.worldViewProj = model.GetWorldMatrix() * Camera::GetCascade();

	mShadowBuffer.Bind(buffer);
}

int ShadowMap::GetResolution()
{
	return mResolution;
}

void ShadowMap::SetResolution(int resolution)
{
	mResolution = resolution;
	UpdateTexture();
}

ID3D11Buffer* const* ShadowMap::GetBuffer()
{
	return mShadowBuffer.Get();
}

ID3D11DepthStencilView* ShadowMap::GetDSV()
{
	return mDepthStencilView;
}

ID3D11ShaderResourceView* const* ShadowMap::GetSRV()
{
	return &mShaderResourceView;
}

void ShadowMap::Shotdown()
{
	SAFE_RELEASE(mShaderResourceView);
	SAFE_RELEASE(mDepthStencilView);

	mShadowBuffer.Release();
}