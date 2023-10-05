#include "PipelineState.h"

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

RasterizerState PipelineState::mRasterizerState;
SamplerState PipelineState::mSamplerState;
DepthStencilState PipelineState::mDepthStencilState;

Array<ID3D11RenderTargetView*> PipelineState::mRenderTargetView;
Array<ID3D11ShaderResourceView*> PipelineState::mShaderResourceView;

ConstantBuffer<PipelineState::PrePassBuffer> PipelineState::mPrePassBuffer;
ConstantBuffer<PipelineState::PassBuffer> PipelineState::mPassBuffer;
ConstantBuffer<PipelineState::LightBuffer> PipelineState::mLightBuffer;

VertexShader PipelineState::mPrePassVS;
PixelShader PipelineState::mPrePassPS;
VertexShader PipelineState::mPassVS;
PixelShader PipelineState::mPassPS;
InputLayout PipelineState::mPrePassIL;
InputLayout PipelineState::mPassIL;
VertexShader PipelineState::mPostProcessVS;
PixelShader PipelineState::mPostProcessPS;
InputLayout PipelineState::mPostProcessIL;

void PipelineState::Setup()
{
	mRenderTargetView.Add(ColorMap::GetRTV());
	mRenderTargetView.Add(SelectMap::GetRTV());
	mRenderTargetView.Add(NormalMap::GetRTV());

	mPrePassBuffer.Create();
	mPassBuffer.Create();
	mLightBuffer.Create();
	mSamplerState.Create();
	mDepthStencilState.Create();
	mRasterizerState.Create();

	{
		mPrePassVS.Create(L"PrePass.hlsl");
		mPrePassPS.Create(L"PrePass.hlsl");

		Array<String> input;
		input.Add(L"POSITION");
		input.Add(L"NORMAL");
		mPrePassIL.Create(mPrePassVS.GetBlob(), input);
	}

	{
		mPassVS.Create(L"Pass.hlsl");
		mPassPS.Create(L"Pass.hlsl");

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
	mRasterizerState.Release();
	mDepthStencilState.Release();
	mLightBuffer.Release();
	mSamplerState.Release();
	mPrePassBuffer.Release();
	mPassBuffer.Release();
	mPrePassVS.Release();
	mPrePassPS.Release();
	mPrePassIL.Release();
	mPassVS.Release();
	mPassPS.Release();
	mPassIL.Release();
	mPostProcessVS.Release();
	mPostProcessPS.Release();
	mPostProcessIL.Release();
}

void PipelineState::Bind()
{
	UpdatePrePassViewport();

	DepthMap::Clear();
	ColorMap::Clear();
	SelectMap::Clear();
	NormalMap::Clear();

	Direct3D::DeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Direct3D::DeviceContext()->RSSetState(mRasterizerState.Get());
	Direct3D::DeviceContext()->OMSetRenderTargets(mRenderTargetView.Size(), mRenderTargetView.Data(), DepthMap::GetDSV());
	Direct3D::DeviceContext()->OMSetDepthStencilState(mDepthStencilState.Get(), 0);

	Direct3D::DeviceContext()->IASetInputLayout(mPrePassIL.Get());
	Direct3D::DeviceContext()->VSSetShader(mPrePassVS.Get(), 0, 0);
	Direct3D::DeviceContext()->PSSetShader(mPrePassPS.Get(), 0, 0);

	Direct3D::DeviceContext()->VSSetConstantBuffers(0, 1, mPrePassBuffer.Get());
	Direct3D::DeviceContext()->PSSetConstantBuffers(1, 1, ColorMap::GetBuffer());
	Direct3D::DeviceContext()->PSSetConstantBuffers(2, 1, SelectMap::GetBuffer());

	static LightBuffer buffer{};
	buffer.cameraPos = Camera::GetPosition();
	buffer.dirCount = 0;
	buffer.pointCount = 0;

	int size = ObjectManager::Size();
	for (int i = 0; i < size; i++)
	{
		Object& obj = ObjectManager::Get(i);
		TypeObject type = obj.GetType();

		switch (type)
		{
			case TypeObject::DirectionalLight:
			{
				DirectionalLight& light = (DirectionalLight&)obj;

				buffer.dirLight[buffer.dirCount].color = light.color;
				buffer.dirLight[buffer.dirCount].dir = light.direction;
				buffer.dirLight[buffer.dirCount].power = light.power;
				buffer.dirCount++;

				break;
			}

			case TypeObject::PointLight:
			{
				PointLight& light = (PointLight&)obj;

				buffer.pointLight[buffer.pointCount].color = light.color;
				buffer.pointLight[buffer.pointCount].position = light.position;
				buffer.pointLight[buffer.pointCount].radius = light.radius;
				buffer.pointLight[buffer.pointCount].power = light.power;
				buffer.pointCount++;

				break;
			}
		}
	}

	for (int i = 0; i < size; i++)
	{
		Object& obj = ObjectManager::Get(i);
		TypeObject type = obj.GetType();

		switch (type)
		{
			case TypeObject::Mesh:
			{
				Mesh& mesh = (Mesh&)obj;

				UpdatePrePassBuffer(mesh);
				ColorMap::UpdateBuffer(mesh);
				SelectMap::UpdateBuffer(mesh);
				
				mesh.Bind();

				break;
			}
		}
	}

	UpdatePassViewport();

	Direct3D::DeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Direct3D::DeviceContext()->RSSetState(mRasterizerState.Get());
	Direct3D::DeviceContext()->PSSetSamplers(0, 1, mSamplerState.Get());
	Direct3D::DeviceContext()->OMSetRenderTargets(1, Direct3D::GetRTV(), 0);

	Direct3D::DeviceContext()->IASetInputLayout(mPassIL.Get());
	Direct3D::DeviceContext()->VSSetShader(mPassVS.Get(), 0, 0);
	Direct3D::DeviceContext()->PSSetShader(mPassPS.Get(), 0, 0);

	Direct3D::DeviceContext()->VSSetConstantBuffers(0, 1, mPassBuffer.Get());
	Direct3D::DeviceContext()->PSSetConstantBuffers(0, 1, mPassBuffer.Get());
	Direct3D::DeviceContext()->PSSetConstantBuffers(1, 1, mLightBuffer.Get());

	Direct3D::DeviceContext()->PSSetShaderResources(0, 1, ColorMap::GetSRV());
	Direct3D::DeviceContext()->PSSetShaderResources(1, 1, DepthMap::GetSRV());
	Direct3D::DeviceContext()->PSSetShaderResources(2, 1, SelectMap::GetSRV());
	Direct3D::DeviceContext()->PSSetShaderResources(3, 1, NormalMap::GetSRV());

	UpdateLightBuffer(buffer);
	UpdatePassBuffer();
	TextureMap::Bind();

	Direct3D::DeviceContext()->PSSetShaderResources(3, 1, Direct3D::NullSRV());
	Direct3D::DeviceContext()->PSSetShaderResources(2, 1, Direct3D::NullSRV());
	Direct3D::DeviceContext()->PSSetShaderResources(1, 1, Direct3D::NullSRV());
	Direct3D::DeviceContext()->PSSetShaderResources(0, 1, Direct3D::NullSRV());
}

void PipelineState::UpdateLightBuffer(LightBuffer& buffer)
{
	mLightBuffer.Bind(buffer);
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

void PipelineState::UpdatePassViewport()
{
	int x, y, width, height;

	if (Application::IsGame())
	{
		x = 0;
		y = 0;
		width = Application::GetGameWidth();
		height = Application::GetGameHeight();
	}
	else
	{
		x = Application::GetSceneX();
		y = Application::GetSceneY();
		width = Application::GetSceneWidth();
		height = Application::GetSceneHeight();
	}

	{
		static D3D11_VIEWPORT viewport{};
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = (FLOAT)x;
		viewport.TopLeftY = (FLOAT)y;
		viewport.Width = (FLOAT)width;
		viewport.Height = (FLOAT)height;

		static D3D11_RECT rect{};
		rect.left = x;
		rect.top = y;
		rect.right = width + x;
		rect.bottom = height + y;

		Direct3D::DeviceContext()->RSSetViewports(1, &viewport);
		Direct3D::DeviceContext()->RSSetScissorRects(1, &rect);
	}
}

void PipelineState::UpdatePrePassBuffer(Mesh& mesh)
{
	Matrix world = mesh.GetWorldMatrix();
	Matrix view = Camera::GetViewMatrix();
	Matrix proj = Camera::GetProjMatrix();
	Matrix inv = mesh.GetWorldInvTransposeMatrix();

	static PrePassBuffer buffer{};
	buffer.worldViewProj = world * view * proj;
	buffer.world = world;
	buffer.worldInvTranspose = inv;

	mPrePassBuffer.Bind(buffer);
}

void PipelineState::UpdatePassBuffer()
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

	static PassBuffer buffer{};
	buffer.width = width;
	buffer.height = height;

	mPassBuffer.Bind(buffer);
}