#include "ColorMap.h"

#include "Application.h"
#include "Utility.h"
#include "PipelineState.h"
#include "Direct3D.h"
#include "ConstantBuffer.h"
#include "Color.h"
#include "Model.h"

ID3D11RenderTargetView* ColorMap::mRenderTargetView = 0;
ID3D11ShaderResourceView* ColorMap::mShaderResourceView = 0;

ConstantBuffer<ColorMap::ColorBuffer> ColorMap::mColorBuffer;

struct ColorMap::ColorBuffer
{
	Color color;
};

void ColorMap::Setup()
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
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		FOG_TRACE(Direct3D::Device()->CreateTexture2D(&desc, 0, &texture));
	}
	{
		D3D11_RENDER_TARGET_VIEW_DESC desc{};
		desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		FOG_TRACE(Direct3D::Device()->CreateRenderTargetView(texture, &desc, &mRenderTargetView));
	}
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc{};
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MostDetailedMip = 0;
		desc.Texture2D.MipLevels = 1;
		FOG_TRACE(Direct3D::Device()->CreateShaderResourceView(texture, &desc, &mShaderResourceView));
	}
	SAFE_RELEASE(texture);

	mColorBuffer.Create();
}

void ColorMap::UpdateBuffer(Model& model)
{
	static ColorBuffer buffer{};
	buffer.color = model.color;

	mColorBuffer.Bind(buffer);
}

void ColorMap::Shotdown()
{
	SAFE_RELEASE(mRenderTargetView);
	SAFE_RELEASE(mShaderResourceView);

	mColorBuffer.Release();
}

ID3D11ShaderResourceView* const* ColorMap::GetSRV()
{
	return &mShaderResourceView;
}

ID3D11RenderTargetView* ColorMap::GetRTV()
{
	return mRenderTargetView;
}

ID3D11Buffer* const* ColorMap::GetBuffer()
{
	return mColorBuffer.Get();
}