#include "PrePass.h"

#include "ConstantBuffer.h"
#include "Matrix.h"
#include "Application.h"
#include "Utility.h"
#include "CustomArray.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "InputLayout.h"
#include "Camera.h"
#include "Model.h"
#include "ObjectManager.h"
#include "Light.h"
#include "LightMap.h"
#include "Vector4.h"
#include "ShadowPass.h"

#include <DirectXMath.h>

struct PrePass::Buffer0
{
	Matrix viewProj;
};

struct PrePass::Buffer1
{
	Matrix world;
};

struct PrePass::Buffer2
{
	Color color;
	unsigned int id;
	bool lighting;
	float metallic;
	float roughness;
};

Array<ID3D11RenderTargetView*> PrePass::mRTV;

ConstantBuffer<PrePass::Buffer0> PrePass::mBuffer0;
ConstantBuffer<PrePass::Buffer1> PrePass::mBuffer1;
ConstantBuffer<PrePass::Buffer2> PrePass::mBuffer2;

VertexShader PrePass::mVertexShader;
PixelShader PrePass::mPixelShader;
InputLayout PrePass::mInputLayout;

ID3D11RenderTargetView* PrePass::mColorRTV = 0;
ID3D11ShaderResourceView* PrePass::mColorSRV = 0;

ID3D11RenderTargetView* PrePass::mNormalLightingRTV = 0;
ID3D11ShaderResourceView* PrePass::mNormalLightingSRV = 0;

ID3D11RenderTargetView* PrePass::mPositionMaterialRTV = 0;
ID3D11ShaderResourceView* PrePass::mPositionMaterialSRV = 0;

ID3D11RenderTargetView* PrePass::mIDRTV = 0;
ID3D11ShaderResourceView* PrePass::mIDSRV = 0;

ID3D11DepthStencilView* PrePass::mDepthDSV = 0;
ID3D11ShaderResourceView* PrePass::mDepthSRV = 0;

void PrePass::Setup()
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
		FOG_TRACE(Direct3D::Device()->CreateRenderTargetView(texture, &desc, &mColorRTV));
	}
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc{};
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MostDetailedMip = 0;
		desc.Texture2D.MipLevels = 1;
		FOG_TRACE(Direct3D::Device()->CreateShaderResourceView(texture, &desc, &mColorSRV));
	}
	SAFE_RELEASE(texture);
	{
		D3D11_TEXTURE2D_DESC desc{};
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R10G10B10A2_UNORM;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		FOG_TRACE(Direct3D::Device()->CreateTexture2D(&desc, 0, &texture));
	}
	{
		D3D11_RENDER_TARGET_VIEW_DESC desc{};
		desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		desc.Format = DXGI_FORMAT_R10G10B10A2_UNORM;
		FOG_TRACE(Direct3D::Device()->CreateRenderTargetView(texture, &desc, &mNormalLightingRTV));
	}
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc{};
		desc.Format = DXGI_FORMAT_R10G10B10A2_UNORM;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MostDetailedMip = 0;
		desc.Texture2D.MipLevels = 1;
		FOG_TRACE(Direct3D::Device()->CreateShaderResourceView(texture, &desc, &mNormalLightingSRV));
	}
	SAFE_RELEASE(texture);
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
		FOG_TRACE(Direct3D::Device()->CreateDepthStencilView(texture, &desc, &mDepthDSV));
	}
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc{};
		desc.Format = DXGI_FORMAT_R32_FLOAT;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MostDetailedMip = 0;
		desc.Texture2D.MipLevels = 1;
		FOG_TRACE(Direct3D::Device()->CreateShaderResourceView(texture, &desc, &mDepthSRV));
	}
	SAFE_RELEASE(texture);
	{
		D3D11_TEXTURE2D_DESC desc{};
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R16G16B16A16_UINT;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		FOG_TRACE(Direct3D::Device()->CreateTexture2D(&desc, 0, &texture));
	}
	{
		D3D11_RENDER_TARGET_VIEW_DESC desc{};
		desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		desc.Format = DXGI_FORMAT_R16G16B16A16_UINT;
		FOG_TRACE(Direct3D::Device()->CreateRenderTargetView(texture, &desc, &mPositionMaterialRTV));
	}
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc{};
		desc.Format = DXGI_FORMAT_R16G16B16A16_UINT;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MostDetailedMip = 0;
		desc.Texture2D.MipLevels = 1;
		FOG_TRACE(Direct3D::Device()->CreateShaderResourceView(texture, &desc, &mPositionMaterialSRV));
	}
	SAFE_RELEASE(texture);
	{
		D3D11_TEXTURE2D_DESC desc{};
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R32_UINT;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		FOG_TRACE(Direct3D::Device()->CreateTexture2D(&desc, 0, &texture));
	}
	{
		D3D11_RENDER_TARGET_VIEW_DESC desc{};
		desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		desc.Format = DXGI_FORMAT_R32_UINT;
		FOG_TRACE(Direct3D::Device()->CreateRenderTargetView(texture, &desc, &mIDRTV));
	}
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc{};
		desc.Format = DXGI_FORMAT_R32_UINT;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MostDetailedMip = 0;
		desc.Texture2D.MipLevels = 1;
		FOG_TRACE(Direct3D::Device()->CreateShaderResourceView(texture, &desc, &mIDSRV));
	}
	SAFE_RELEASE(texture);

	{
		mVertexShader.Create(L"PrePass.hlsl");
		mPixelShader.Create(L"PrePass.hlsl");

		mInputLayout.Add(L"POSITION");
		mInputLayout.Add(L"NORMAL");
		mInputLayout.Add(L"TEXCOORD");
		mInputLayout.Create(mVertexShader.GetBlob());
	}

	mBuffer0.Create();
	mBuffer1.Create();
	mBuffer2.Create();

	mRTV.Add(mColorRTV);
	mRTV.Add(mNormalLightingRTV);
	mRTV.Add(mPositionMaterialRTV);
	mRTV.Add(mIDRTV);
}

void PrePass::Bind()
{
	UpdateViewport();
	Clear();

	Direct3D::DeviceContext()->OMSetRenderTargets(mRTV.Size(), mRTV.Data(), mDepthDSV);

	Direct3D::DeviceContext()->IASetInputLayout(mInputLayout.Get());
	Direct3D::DeviceContext()->VSSetShader(mVertexShader.Get(), 0, 0);
	Direct3D::DeviceContext()->PSSetShader(mPixelShader.Get(), 0, 0);

	Direct3D::DeviceContext()->VSSetConstantBuffers(0, 1, mBuffer0.Get());
	Direct3D::DeviceContext()->VSSetConstantBuffers(1, 1, mBuffer1.Get());
	Direct3D::DeviceContext()->PSSetConstantBuffers(2, 1, mBuffer2.Get());

	UpdateBuffer0();

	int size = ObjectManager::Size<DirectionLight>();
	for (int i = 0; i < size; i++)
	{
		DirectionLight* light = ObjectManager::GetWithNumber<DirectionLight>(i);
		if (!light->GetEnable()) continue;

		LightMap::UpdateBuffer(light);

		UpdateBuffer1(light->GetModel());
		UpdateBuffer2(light->GetModel());

		light->BindTexture();
		light->Bind();

		break;
	}

	size = ObjectManager::Size<PointLight>();
	for (int i = 0; i < size; i++)
	{
		PointLight* light = ObjectManager::GetWithNumber<PointLight>(i);
		if (!light->GetEnable()) continue;

		int count = LightMap::UpdateBuffer(light);

		UpdateBuffer1(light->GetModel());
		UpdateBuffer2(light->GetModel());

		light->BindTexture();
		light->Bind();

		if (count == MAX_POINT_LIGHT) break;
	}

	size = ObjectManager::Size<Model>();
	for (int i = 0; i < size; i++)
	{
		Model* model = ObjectManager::GetWithNumber<Model>(i);

		UpdateBuffer1(model);
		UpdateBuffer2(model);

		model->BindTexture();
		model->Draw();
	}
}

void PrePass::UpdateViewport()
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

	static D3D11_VIEWPORT viewport{};
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = (FLOAT)width;
	viewport.Height = (FLOAT)height;

	static D3D11_RECT rect{};
	rect.left = 0;
	rect.top = 0;
	rect.right = width;
	rect.bottom = height;

	Direct3D::DeviceContext()->RSSetViewports(1, &viewport);
	Direct3D::DeviceContext()->RSSetScissorRects(1, &rect);
}

void PrePass::UpdateBuffer0()
{
	Matrix view = Camera::GetViewMatrix();
	Matrix proj = Camera::GetProjMatrix();

	static Buffer0 buffer{};
	buffer.viewProj = view * proj;

	mBuffer0.Bind(buffer);
}

void PrePass::UpdateBuffer1(Model* model)
{
	static Buffer1 buffer{};
	buffer.world = model->GetWorldMatrix();

	mBuffer1.Bind(buffer);
}

void PrePass::UpdateBuffer2(Model* model)
{
	static Buffer2 buffer{};
	buffer.color = model->GetColor();
	buffer.id = model->GetID();
	buffer.lighting = model->GetLighting();
	buffer.roughness = model->GetRoughness();
	buffer.metallic = model->GetMetallic();

	mBuffer2.Bind(buffer);
}

void PrePass::Clear()
{
	static const float color[4]{ 0.0f, 0.0f, 0.0f, 0.0f };

	Direct3D::DeviceContext()->ClearDepthStencilView(mDepthDSV, D3D11_CLEAR_DEPTH, 0.0f, 0);
	Direct3D::DeviceContext()->ClearRenderTargetView(mColorRTV, color);
	Direct3D::DeviceContext()->ClearRenderTargetView(mNormalLightingRTV, color);
	Direct3D::DeviceContext()->ClearRenderTargetView(mPositionMaterialRTV, color);
	Direct3D::DeviceContext()->ClearRenderTargetView(mIDRTV, color);
}

ID3D11DepthStencilView* const PrePass::GetDepthDSV()
{
	return mDepthDSV;
}

ID3D11RenderTargetView* const* PrePass::GetColorRTV()
{
	return &mColorRTV;
}

ID3D11RenderTargetView* const* PrePass::GetNormalLightingRTV()
{
	return &mNormalLightingRTV;
}

ID3D11RenderTargetView* const* PrePass::GetIDRTV()
{
	return &mIDRTV;
}

ID3D11RenderTargetView* const* PrePass::GetPositionMaterialRTV()
{
	return &mPositionMaterialRTV;
}

ID3D11ShaderResourceView* const* PrePass::GetDepthSRV()
{
	return &mDepthSRV;
}

ID3D11ShaderResourceView* const* PrePass::GetColorSRV()
{
	return &mColorSRV;
}

ID3D11ShaderResourceView* const* PrePass::GetNormalLightingSRV()
{
	return &mNormalLightingSRV;
}

ID3D11ShaderResourceView* const* PrePass::GetIDSRV()
{
	return &mIDSRV;
}

ID3D11ShaderResourceView* const* PrePass::GetPositionMaterialSRV()
{
	return &mPositionMaterialSRV;
}

void PrePass::Shotdown()
{
	SAFE_RELEASE(mColorRTV);
	SAFE_RELEASE(mColorSRV);
	SAFE_RELEASE(mNormalLightingRTV);
	SAFE_RELEASE(mNormalLightingSRV);
	SAFE_RELEASE(mDepthDSV);
	SAFE_RELEASE(mDepthSRV);
	SAFE_RELEASE(mPositionMaterialRTV);
	SAFE_RELEASE(mPositionMaterialSRV);
	SAFE_RELEASE(mIDRTV);
	SAFE_RELEASE(mIDSRV);

	mBuffer0.Release();
	mBuffer1.Release();
	mBuffer2.Release();
	mPixelShader.Release();
	mVertexShader.Release();
	mInputLayout.Release();
}