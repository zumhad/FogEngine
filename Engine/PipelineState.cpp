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
#include "ShadowPass.h"
#include "Matrix.h"
#include "Model.h"
#include "SkyboxPass.h"
#include "Light.h"
#include "Matrix3.h"
#include "OutlinePass.h"
#include "PrePass.h"

RasterizerState PipelineState::mShadowRasterizerState;
RasterizerState PipelineState::mRasterizerState;
SamplerState PipelineState::mShadowSamplerState;
SamplerState PipelineState::mSamplerState;
SamplerState PipelineState::mPostProcessSamplerState;
DepthStencilState PipelineState::mReadWriteDSS;
DepthStencilState PipelineState::mReadOnlyDSS;
DepthStencilState PipelineState::mDisableDSS;
RasterizerState PipelineState::mSkyboxRasterizerState;

VertexShader PipelineState::mPassVS;
PixelShader PipelineState::mPassPS;
InputLayout PipelineState::mPassIL;

VertexShader PipelineState::mPostProcessVS;
PixelShader PipelineState::mPostProcessPS;
InputLayout PipelineState::mPostProcessIL;

void PipelineState::Setup()
{
	mSamplerState.Create();
	mShadowSamplerState.Create(SamplerStateType::Shadow);
	mPostProcessSamplerState.Create(SamplerStateType::PostProcess);

	mReadWriteDSS.Create(DepthStencilStateType::ReadWrite);
	mReadOnlyDSS.Create(DepthStencilStateType::ReadOnly);
	mDisableDSS.Create(DepthStencilStateType::Disable);

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
}

void PipelineState::Shotdown()
{
	mDisableDSS.Release();
	mReadWriteDSS.Release();
	mReadOnlyDSS.Release();
	mSkyboxRasterizerState.Release();
	mPostProcessSamplerState.Release();
	mShadowSamplerState.Release();
	mShadowRasterizerState.Release();
	mRasterizerState.Release();
	mSamplerState.Release();
	mPassVS.Release();
	mPassPS.Release();
	mPassIL.Release();
	mPostProcessVS.Release();
	mPostProcessPS.Release();
	mPostProcessIL.Release();
}

void PipelineState::Bind()
{
	Direct3D::DeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//////////////////////////////////////////////////////////////////////////////

	Direct3D::DeviceContext()->RSSetState(mShadowRasterizerState.Get());
	Direct3D::DeviceContext()->OMSetDepthStencilState(mReadWriteDSS.Get(), 0);

	ShadowPass::Bind();

	//////////////////////////////////////////////////////////////////////////////////////////

	Direct3D::DeviceContext()->RSSetState(mRasterizerState.Get());
	Direct3D::DeviceContext()->PSSetSamplers(0, 1, mSamplerState.Get());
	Direct3D::DeviceContext()->OMSetDepthStencilState(mReadWriteDSS.Get(), 0);

	PrePass::Bind();

	//////////////////////////////////////////////////////////////////////////////////////////////

	Direct3D::DeviceContext()->RSSetState(mRasterizerState.Get());
	Direct3D::DeviceContext()->PSSetSamplers(0, 1, mSamplerState.Get());
	Direct3D::DeviceContext()->PSSetSamplers(1, 1, mShadowSamplerState.Get());
	Direct3D::DeviceContext()->OMSetRenderTargets(1, LightMap::GetRTV(), 0);
	Direct3D::DeviceContext()->OMSetDepthStencilState(mDisableDSS.Get(), 0);

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
	Direct3D::DeviceContext()->PSSetShaderResources(5, 1, ShadowPass::GetDepthSRV());

	LightMap::UpdateBuffer();
	TextureMap::Bind();

	Direct3D::DeviceContext()->PSSetShaderResources(5, 1, Direct3D::NullSRV());
	Direct3D::DeviceContext()->PSSetShaderResources(4, 1, Direct3D::NullSRV());
	Direct3D::DeviceContext()->PSSetShaderResources(3, 1, Direct3D::NullSRV());
	Direct3D::DeviceContext()->PSSetShaderResources(2, 1, Direct3D::NullSRV());
	Direct3D::DeviceContext()->PSSetShaderResources(1, 1, Direct3D::NullSRV());
	Direct3D::DeviceContext()->PSSetShaderResources(0, 1, Direct3D::NullSRV());

	//////////////////////////////////////////////////////////////////////////////////////////////

	Direct3D::DeviceContext()->RSSetState(mSkyboxRasterizerState.Get());
	Direct3D::DeviceContext()->PSSetSamplers(0, 1, mSamplerState.Get());
	Direct3D::DeviceContext()->OMSetRenderTargets(1, LightMap::GetRTV(), PrePass::GetDepthDSV());
	Direct3D::DeviceContext()->OMSetDepthStencilState(mReadOnlyDSS.Get(), 0);

	SkyboxPass::Bind();

	////////////////////////////////////////////////////////////////////////////////////////////////////////

	Direct3D::DeviceContext()->RSSetState(mRasterizerState.Get());
	Direct3D::DeviceContext()->OMSetDepthStencilState(mDisableDSS.Get(), 0);

	OutlinePass::Bind();

	////////////////////////////////////////////////////////////////////////////////////////////////////////

	Direct3D::DeviceContext()->RSSetState(mRasterizerState.Get());
	Direct3D::DeviceContext()->PSSetSamplers(0, 1, mPostProcessSamplerState.Get());
	Direct3D::DeviceContext()->OMSetRenderTargets(1, Direct3D::GetRTV(), 0);
	Direct3D::DeviceContext()->OMSetDepthStencilState(mDisableDSS.Get(), 0);

	Direct3D::DeviceContext()->IASetInputLayout(mPostProcessIL.Get());
	Direct3D::DeviceContext()->VSSetShader(mPostProcessVS.Get(), 0, 0);
	Direct3D::DeviceContext()->PSSetShader(mPostProcessPS.Get(), 0, 0);

	Direct3D::DeviceContext()->PSSetConstantBuffers(0, 1, PostProcess::GetBuffer());

	Direct3D::DeviceContext()->PSSetShaderResources(0, 1, LightMap::GetSRV());

	PostProcess::UpdateBuffer();
	TextureMap::Bind();

	Direct3D::DeviceContext()->PSSetShaderResources(0, 1, Direct3D::NullSRV());
}