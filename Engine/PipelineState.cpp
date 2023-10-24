#include "PipelineState.h"

#include "Application.h"
#include "Direct3D.h"
#include "PixelShader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "RasterizerState.h"
#include "ConstantBuffer.h"
#include "ObjectManager.h"
#include "ColorMap.h"
#include "Camera.h"
#include "TextureMap.h"
#include "DepthMap.h"
#include "SelectMap.h"
#include "Texture.h"
#include "SamplerState.h"
#include "DepthStencilState.h"
#include "VertexShader.h"
#include "InputLayout.h"
#include "NormalMap.h"
#include "LightMap.h"
#include "PostProcess.h"
#include "ShadowMap.h"
#include "Matrix.h"
#include "Model.h"

RasterizerState PipelineState::mShadowRasterizerState;
RasterizerState PipelineState::mRasterizerState;
SamplerState PipelineState::mShadowSamplerState;
SamplerState PipelineState::mSamplerState;
SamplerState PipelineState::mPostProcessSamplerState;
DepthStencilState PipelineState::mDepthStencilState;

Array<ID3D11RenderTargetView*> PipelineState::mRenderTargetView;
Array<ID3D11ShaderResourceView*> PipelineState::mShaderResourceView;

ConstantBuffer<PipelineState::PrePassBuffer> PipelineState::mPrePassBuffer;

VertexShader PipelineState::mPrePassVS;
PixelShader PipelineState::mPrePassPS;
InputLayout PipelineState::mPrePassIL;

VertexShader PipelineState::mPassVS;
PixelShader PipelineState::mPassPS;
InputLayout PipelineState::mPassIL;

VertexShader PipelineState::mPostProcessVS;
PixelShader PipelineState::mPostProcessPS;
InputLayout PipelineState::mPostProcessIL;

VertexShader PipelineState::mShadowPassVS;
PixelShader PipelineState::mShadowPassPS;
InputLayout PipelineState::mShadowPassIL;

struct PipelineState::PrePassBuffer
{
	Matrix worldViewProj;
	Matrix world;
	Matrix worldInvTranspose;
};

void PipelineState::Setup()
{
	mRenderTargetView.Add(ColorMap::GetRTV());
	mRenderTargetView.Add(SelectMap::GetRTV());
	mRenderTargetView.Add(NormalMap::GetRTV());

	mPrePassBuffer.Create();

	mSamplerState.Create();
	mShadowSamplerState.Create(SamplerStateType::Shadow);
	mPostProcessSamplerState.Create(SamplerStateType::PostProcess);
	mDepthStencilState.Create();
	mRasterizerState.Create();
	mShadowRasterizerState.Create(RasterizerStateType::Shadow);

	{
		mPrePassVS.Create(L"PrePass.hlsl");
		mPrePassPS.Create(L"PrePass.hlsl");

		Array<String> input;
		input.Add(L"POSITION");
		input.Add(L"NORMAL");
		input.Add(L"TEXCOORD");
		mPrePassIL.Create(mPrePassVS.GetBlob(), input);
	}

	{
		mPassVS.Create(L"LightPass.hlsl");
		mPassPS.Create(L"LightPass.hlsl");

		Array<String> input;
		input.Add(L"TEXCOORD");
		mPassIL.Create(mPassVS.GetBlob(), input);
	}

	{
		mPostProcessVS.Create(L"PostProcess.hlsl");
		mPostProcessPS.Create(L"PostProcess.hlsl");

		Array<String> input;
		input.Add(L"TEXCOORD");
		mPostProcessIL.Create(mPostProcessVS.GetBlob(), input);
	}

	{
		mShadowPassVS.Create(L"ShadowPass.hlsl");
		mShadowPassPS.Create(L"ShadowPass.hlsl");

		Array<String> input;
		input.Add(L"POSITION");
		mShadowPassIL.Create(mShadowPassVS.GetBlob(), input);
	}
}

void PipelineState::Shotdown()
{
	mPostProcessSamplerState.Release();
	mShadowSamplerState.Release();
	mShadowRasterizerState.Release();
	mRasterizerState.Release();
	mDepthStencilState.Release();
	mSamplerState.Release();
	mPrePassBuffer.Release();
	mPrePassVS.Release();
	mPrePassPS.Release();
	mPrePassIL.Release();
	mPassVS.Release();
	mPassPS.Release();
	mPassIL.Release();
	mPostProcessVS.Release();
	mPostProcessPS.Release();
	mPostProcessIL.Release();
	mShadowPassVS.Release();
	mShadowPassPS.Release();
	mShadowPassIL.Release();
}

void PipelineState::Bind()
{
	UpdateShadowPassViewport();

	ShadowMap::Clear();

	Direct3D::DeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Direct3D::DeviceContext()->RSSetState(mShadowRasterizerState.Get());
	Direct3D::DeviceContext()->OMSetRenderTargets(0, 0, ShadowMap::GetDSV());
	Direct3D::DeviceContext()->OMSetDepthStencilState(mDepthStencilState.Get(), 0);
	
	Direct3D::DeviceContext()->IASetInputLayout(mShadowPassIL.Get());
	Direct3D::DeviceContext()->VSSetShader(mShadowPassVS.Get(), 0, 0);
	Direct3D::DeviceContext()->PSSetShader(mShadowPassPS.Get(), 0, 0);

	Direct3D::DeviceContext()->VSSetConstantBuffers(0, 1, ShadowMap::GetBuffer());

	int size = ObjectManager::Size();
	for (int i = 0; i < size; i++)
	{
		Object* obj = ObjectManager::Get<Object>(i);
		TypeObject type = obj->GetType();

		switch (type)
		{
			case TypeObject::Model:
			{
				Model* model = ObjectManager::Get<Model>(obj);

				ShadowMap::UpdateBuffer(*model);

				model->Draw();

				break;
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////

	UpdatePrePassViewport();

	DepthMap::Clear();
	ColorMap::Clear();
	SelectMap::Clear();
	NormalMap::Clear();

	Direct3D::DeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Direct3D::DeviceContext()->RSSetState(mRasterizerState.Get());
	Direct3D::DeviceContext()->PSSetSamplers(0, 1, mSamplerState.Get());
	Direct3D::DeviceContext()->OMSetRenderTargets(mRenderTargetView.Size(), mRenderTargetView.Data(), DepthMap::GetDSV());
	Direct3D::DeviceContext()->OMSetDepthStencilState(mDepthStencilState.Get(), 0);

	Direct3D::DeviceContext()->IASetInputLayout(mPrePassIL.Get());
	Direct3D::DeviceContext()->VSSetShader(mPrePassVS.Get(), 0, 0);
	Direct3D::DeviceContext()->PSSetShader(mPrePassPS.Get(), 0, 0);

	Direct3D::DeviceContext()->VSSetConstantBuffers(0, 1, mPrePassBuffer.Get());
	Direct3D::DeviceContext()->PSSetConstantBuffers(1, 1, ColorMap::GetBuffer());
	Direct3D::DeviceContext()->PSSetConstantBuffers(2, 1, SelectMap::GetBuffer());

	for (int i = 0; i < size; i++)
	{
		Object* obj = ObjectManager::Get<Object>(i);
		TypeObject type = obj->GetType();

		switch (type)
		{
			case TypeObject::DirectionalLight:
			{
				DirectionalLight* light = ObjectManager::Get<DirectionalLight>(obj);
				LightMap::UpdateBuffer(*light);

				break;
			}

			case TypeObject::PointLight:
			{
				PointLight* light = ObjectManager::Get<PointLight>(obj);
				LightMap::UpdateBuffer(*light);

				break;
			}

			case TypeObject::Model:
			{
				Model* model = ObjectManager::Get<Model>(obj);

				UpdatePrePassBuffer(*model);
				ColorMap::UpdateBuffer(*model);
				SelectMap::UpdateBuffer(*model);

				model->BindTexture();
				model->Draw();

				break;
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////////

	Direct3D::DeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Direct3D::DeviceContext()->RSSetState(mRasterizerState.Get());
	Direct3D::DeviceContext()->PSSetSamplers(0, 1, mSamplerState.Get());
	Direct3D::DeviceContext()->PSSetSamplers(1, 1, mShadowSamplerState.Get());
	Direct3D::DeviceContext()->OMSetRenderTargets(1, LightMap::GetRTV(), 0);

	Direct3D::DeviceContext()->IASetInputLayout(mPassIL.Get());
	Direct3D::DeviceContext()->VSSetShader(mPassVS.Get(), 0, 0);
	Direct3D::DeviceContext()->PSSetShader(mPassPS.Get(), 0, 0);

	Direct3D::DeviceContext()->VSSetConstantBuffers(0, 1, LightMap::GetBuffer());
	Direct3D::DeviceContext()->PSSetConstantBuffers(0, 1, LightMap::GetBuffer());

	Direct3D::DeviceContext()->PSSetShaderResources(0, 1, ColorMap::GetSRV());
	Direct3D::DeviceContext()->PSSetShaderResources(1, 1, DepthMap::GetSRV());
	Direct3D::DeviceContext()->PSSetShaderResources(2, 1, SelectMap::GetSRV());
	Direct3D::DeviceContext()->PSSetShaderResources(3, 1, NormalMap::GetSRV());
	Direct3D::DeviceContext()->PSSetShaderResources(4, 1, ShadowMap::GetSRV());

	LightMap::UpdateBuffer();
	TextureMap::Bind();

	Direct3D::DeviceContext()->PSSetShaderResources(4, 1, Direct3D::NullSRV());
	Direct3D::DeviceContext()->PSSetShaderResources(3, 1, Direct3D::NullSRV());
	Direct3D::DeviceContext()->PSSetShaderResources(2, 1, Direct3D::NullSRV());
	Direct3D::DeviceContext()->PSSetShaderResources(1, 1, Direct3D::NullSRV());
	Direct3D::DeviceContext()->PSSetShaderResources(0, 1, Direct3D::NullSRV());

	////////////////////////////////////////////////////////////////////////////////////////////////////////

	Direct3D::DeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Direct3D::DeviceContext()->RSSetState(mRasterizerState.Get());
	Direct3D::DeviceContext()->PSSetSamplers(0, 1, mPostProcessSamplerState.Get());
	Direct3D::DeviceContext()->OMSetRenderTargets(1, Direct3D::GetRTV(), 0);

	Direct3D::DeviceContext()->IASetInputLayout(mPostProcessIL.Get());
	Direct3D::DeviceContext()->VSSetShader(mPostProcessVS.Get(), 0, 0);
	Direct3D::DeviceContext()->PSSetShader(mPostProcessPS.Get(), 0, 0);

	Direct3D::DeviceContext()->PSSetConstantBuffers(0, 1, PostProcess::GetBuffer());

	Direct3D::DeviceContext()->PSSetShaderResources(0, 1, LightMap::GetSRV());

	PostProcess::UpdateBuffer();
	TextureMap::Bind();

	Direct3D::DeviceContext()->PSSetShaderResources(0, 1, Direct3D::NullSRV());
}

void PipelineState::UpdatePrePassViewport()
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

	{
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
}

void PipelineState::UpdateShadowPassViewport()
{
	int width = ShadowMap::GetResolution();
	int height = ShadowMap::GetResolution();

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

void PipelineState::UpdatePrePassBuffer(Model& model)
{
	Matrix world = model.GetWorldMatrix();
	Matrix view = Camera::GetViewMatrix();
	Matrix proj = Camera::GetProjMatrix();
	Matrix inv = model.GetWorldInvTransposeMatrix(world);

	static PrePassBuffer buffer{};
	buffer.worldViewProj = world * view * proj;
	buffer.world = world;
	buffer.worldInvTranspose = inv;

	mPrePassBuffer.Bind(buffer);
}