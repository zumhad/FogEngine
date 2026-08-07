#include "DepthPass.h"

#pragma warning(disable : 6387)

#include "Utility.h"
#include "Direct3D.h"
#include "Application.h"
#include "Matrix.h"
#include "ConstantBuffer.h"
#include "Camera.h"
#include "Model.h"
#include "VertexShader.h"
#include "InputLayout.h"
#include "ObjectManager.h"
#include "Light.h"

struct DepthPass::Buffer0
{
	Matrix viewProj;
};

struct DepthPass::Buffer1
{
	Matrix world;
};

VertexShader DepthPass::mVertexShader;
InputLayout DepthPass::mInputLayout;

ConstantBuffer<DepthPass::Buffer0> DepthPass::mBuffer0;
ConstantBuffer<DepthPass::Buffer1> DepthPass::mBuffer1;

ID3D11DepthStencilView* DepthPass::mDepthDSV = 0;
ID3D11ShaderResourceView* DepthPass::mDepthSRV = 0;

void DepthPass::Setup()
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
		mVertexShader.Create(L"DepthPass.hlsl");

		mInputLayout.Add(L"POSITION");
		mInputLayout.Create(mVertexShader.GetBlob());
	}

	mBuffer0.Create();
	mBuffer1.Create();
}

void DepthPass::Bind()
{
	Clear();

	Direct3D::DeviceContext()->OMSetRenderTargets(0, 0, mDepthDSV);

	Direct3D::DeviceContext()->IASetInputLayout(mInputLayout.Get());
	Direct3D::DeviceContext()->VSSetShader(mVertexShader.Get(), 0, 0);
	Direct3D::DeviceContext()->PSSetShader(0, 0, 0);

	Direct3D::DeviceContext()->VSSetConstantBuffers(0, 1, mBuffer0.Get());
	Direct3D::DeviceContext()->VSSetConstantBuffers(1, 1, mBuffer1.Get());

	UpdateBuffer0();

	int size = ObjectManager::Size<DirectionLight>();
	for (int i = 0; i < size; i++)
	{
		Model* model = ObjectManager::GetWithNumber<DirectionLight>(i)->GetModel();

		UpdateBuffer1(model);

		model->Bind(false, false);
	}

	size = ObjectManager::Size<PointLight>();
	for (int i = 0; i < size; i++)
	{
		Model* model = ObjectManager::GetWithNumber<PointLight>(i)->GetModel();

		UpdateBuffer1(model);

		model->Bind(false, false);
	}

	size = ObjectManager::Size<Model>();
	for (int i = 0; i < size; i++)
	{
		Model* model = ObjectManager::GetWithNumber<Model>(i);

		UpdateBuffer1(model);

		model->Bind(false, false);
	}
}

void DepthPass::UpdateBuffer0()
{
	static Buffer0 buffer;
	buffer.viewProj = Camera::GetViewMatrix() * Camera::GetProjMatrix();

	mBuffer0.Bind(buffer);
}

void DepthPass::UpdateBuffer1(Model* model)
{
	static Buffer1 buffer;
	buffer.world = model->GetWorldMatrix();

	mBuffer1.Bind(buffer);
}

void DepthPass::Clear()
{
	Direct3D::DeviceContext()->ClearDepthStencilView(mDepthDSV, D3D11_CLEAR_DEPTH, 0.0f, 0);
}

ID3D11DepthStencilView* const DepthPass::GetDepthDSV()
{
	return mDepthDSV;
}

ID3D11ShaderResourceView* const* DepthPass::GetDepthSRV()
{
	return &mDepthSRV;
}

void DepthPass::Shotdown()
{
	mVertexShader.Release();
	mInputLayout.Release();
	mBuffer0.Release();
	mBuffer1.Release();

	SAFE_RELEASE(mDepthDSV);
	SAFE_RELEASE(mDepthSRV);
}