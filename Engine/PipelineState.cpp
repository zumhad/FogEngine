#include "PipelineState.h"

#include "Application.h"
#include "Direct3D.h"
#include "PixelShader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "RasterizerState.h"
#include "ConstantBuffer.h"
#include "ObjectManager.h"
#include "Camera.h"
#include "TextureMap.h"
#include "Texture.h"
#include "SamplerState.h"
#include "DepthStencilState.h"
#include "VertexShader.h"
#include "InputLayout.h"
#include "LightMap.h"
#include "PostProcess.h"
#include "ShadowMap.h"
#include "Matrix.h"
#include "Model.h"
#include "Skybox.h"
#include "Light.h"
#include "Matrix3.h"
#include "OutlineMap.h"
#include "PrePass.h"

RasterizerState PipelineState::mShadowRasterizerState;
RasterizerState PipelineState::mRasterizerState;
SamplerState PipelineState::mShadowSamplerState;
SamplerState PipelineState::mSamplerState;
SamplerState PipelineState::mPostProcessSamplerState;
DepthStencilState PipelineState::mDepthStencilState;
DepthStencilState PipelineState::mSkyboxDepthStencilState;
RasterizerState PipelineState::mSkyboxRasterizerState;

VertexShader PipelineState::mPassVS;
PixelShader PipelineState::mPassPS;
InputLayout PipelineState::mPassIL;

VertexShader PipelineState::mPostProcessVS;
PixelShader PipelineState::mPostProcessPS;
InputLayout PipelineState::mPostProcessIL;

VertexShader PipelineState::mShadowPassVS;
PixelShader PipelineState::mShadowPassPS;
InputLayout PipelineState::mShadowPassIL;

void PipelineState::Setup()
{
	mSamplerState.Create();
	mShadowSamplerState.Create(SamplerStateType::Shadow);
	mPostProcessSamplerState.Create(SamplerStateType::PostProcess);
	mDepthStencilState.Create();
	mSkyboxDepthStencilState.Create(DepthStencilStateType::Skybox);
	mSkyboxRasterizerState.Create(RasterizerStateType::Skybox);
	mRasterizerState.Create();
	mShadowRasterizerState.Create(RasterizerStateType::Shadow);

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
	mSkyboxDepthStencilState.Release();
	mSkyboxRasterizerState.Release();
	mPostProcessSamplerState.Release();
	mShadowSamplerState.Release();
	mShadowRasterizerState.Release();
	mRasterizerState.Release();
	mDepthStencilState.Release();
	mSamplerState.Release();
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
	Vector3 lightDir = Vector3(0.0f, 0.0f, 0.0f);

	int size = ObjectManager::Size<DirectionLight>();
	for (int i = 0; i < size; i++)
	{
		DirectionLight* light = ObjectManager::GetWithNumber<DirectionLight>(i);
		if (!light->enable) continue;

		lightDir = light->GetDirection();

		break;
	}

	if (lightDir != Vector3(0.0f, 0.0f, 0.0f))
	{
		UpdateShadowPassViewport();

		ShadowMap::UpdateCascade(lightDir);

		Direct3D::DeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		Direct3D::DeviceContext()->RSSetState(mShadowRasterizerState.Get());
		Direct3D::DeviceContext()->OMSetDepthStencilState(mDepthStencilState.Get(), 0);

		Direct3D::DeviceContext()->IASetInputLayout(mShadowPassIL.Get());
		Direct3D::DeviceContext()->VSSetShader(mShadowPassVS.Get(), 0, 0);
		Direct3D::DeviceContext()->PSSetShader(mShadowPassPS.Get(), 0, 0);

		Direct3D::DeviceContext()->VSSetConstantBuffers(0, 1, ShadowMap::GetBuffer());

		for (int i = 0; i < MAX_CASCADES; i++)
		{
			ShadowMap::Clear(i);

			Direct3D::DeviceContext()->OMSetRenderTargets(0, 0, ShadowMap::GetDSV(i));

			size = ObjectManager::Size<Model>();
			for (int j = 0; j < size; j++)
			{
				Model* model = ObjectManager::GetWithNumber<Model>(j);

				ShadowMap::UpdateBuffer(*model, i);

				model->Draw();
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////

	Direct3D::DeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Direct3D::DeviceContext()->RSSetState(mRasterizerState.Get());
	Direct3D::DeviceContext()->PSSetSamplers(0, 1, mSamplerState.Get());
	Direct3D::DeviceContext()->OMSetDepthStencilState(mDepthStencilState.Get(), 0);

	PrePass::Bind();

	//////////////////////////////////////////////////////////////////////////////////////////////

	Direct3D::DeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Direct3D::DeviceContext()->RSSetState(mSkyboxRasterizerState.Get());
	Direct3D::DeviceContext()->PSSetSamplers(0, 1, mSamplerState.Get());
	Direct3D::DeviceContext()->OMSetRenderTargets(1, PrePass::GetColorRTV(), PrePass::GetDepthDSV());
	Direct3D::DeviceContext()->OMSetDepthStencilState(mSkyboxDepthStencilState.Get(), 0);

	Skybox::Bind();

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

	Direct3D::DeviceContext()->PSSetShaderResources(0, 1, PrePass::GetColorSRV());
	Direct3D::DeviceContext()->PSSetShaderResources(1, 1, PrePass::GetDepthSRV());
	Direct3D::DeviceContext()->PSSetShaderResources(2, 1, PrePass::GetPositionIDSRV());
	Direct3D::DeviceContext()->PSSetShaderResources(3, 1, PrePass::GetNormalLightSRV());
	Direct3D::DeviceContext()->PSSetShaderResources(4, 1, PrePass::GetRangeMaterialSRV());
	Direct3D::DeviceContext()->PSSetShaderResources(5, 1, ShadowMap::GetSRV());

	LightMap::UpdateBuffer();
	TextureMap::Bind();

	Direct3D::DeviceContext()->PSSetShaderResources(5, 1, Direct3D::NullSRV());
	Direct3D::DeviceContext()->PSSetShaderResources(4, 1, Direct3D::NullSRV());
	Direct3D::DeviceContext()->PSSetShaderResources(3, 1, Direct3D::NullSRV());
	Direct3D::DeviceContext()->PSSetShaderResources(2, 1, Direct3D::NullSRV());
	Direct3D::DeviceContext()->PSSetShaderResources(1, 1, Direct3D::NullSRV());
	Direct3D::DeviceContext()->PSSetShaderResources(0, 1, Direct3D::NullSRV());

	////////////////////////////////////////////////////////////////////////////////////////////////////////

	OutlineMap::Clear();

	Direct3D::DeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Direct3D::DeviceContext()->RSSetState(mRasterizerState.Get());

	OutlineMap::Bind();

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