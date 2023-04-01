#include "PassMap.h"

#include "Direct3D.h"
#include "Matrix.h"
#include "Camera.h"
#include "PathHelper.h"
#include "Shader.h"

ConstantBuffer<PassMap::PrePassBuffer> PassMap::mPrePassBuffer;
ConstantBuffer<PassMap::PassBuffer> PassMap::mPassBuffer;

ID3D11VertexShader* PassMap::mVertexShaderPrePass = 0;
ID3D11PixelShader* PassMap::mPixelShaderPrePass = 0;
ID3D11InputLayout* PassMap::mInputLayoutPrePass = 0;

ID3D11VertexShader* PassMap::mVertexShaderPass = 0;
ID3D11PixelShader* PassMap::mPixelShaderPass = 0;
ID3D11InputLayout* PassMap::mInputLayoutPass = 0;

void PassMap::Setup()
{
	mPrePassBuffer.Create();
	mPassBuffer.Create();

	UpdateShader();
}

void PassMap::Shotdown()
{
	mPrePassBuffer.Release();
	mPassBuffer.Release();

	SAFE_RELEASE(mVertexShaderPrePass);
	SAFE_RELEASE(mPixelShaderPrePass);
	SAFE_RELEASE(mInputLayoutPrePass);
	SAFE_RELEASE(mVertexShaderPass);
	SAFE_RELEASE(mPixelShaderPass);
	SAFE_RELEASE(mInputLayoutPass);
}

void PassMap::BindSRV()
{
	float x, y, width, height;

	if (Application::IsGame())
	{
		x = 0.0f;
		y = 0.0f;
		width = (float)Application::GetGameWidth();
		height = (float)Application::GetGameHeight();
	}
	else
	{
		x = (float)Application::GetSceneX();
		y = (float)Application::GetSceneY();
		width = (float)Application::GetSceneWidth();
		height = (float)Application::GetSceneHeight();
	}

	{
		static D3D11_VIEWPORT viewport{};
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		viewport.Width = width;
		viewport.Height = height;

		Direct3D::DeviceContext()->RSSetViewports(1, &viewport);
	}

	Direct3D::DeviceContext()->IASetInputLayout(mInputLayoutPrePass);
	Direct3D::DeviceContext()->VSSetShader(mVertexShaderPrePass, 0, 0);
	Direct3D::DeviceContext()->PSSetShader(mPixelShaderPrePass, 0, 0);

	Direct3D::DeviceContext()->VSSetConstantBuffers(0, 1, mPrePassBuffer.Get());
}

void PassMap::BindRTV()
{
	float x, y, width, height;

	if (Application::IsGame())
	{
		x = 0.0f;
		y = 0.0f;
		width = (float)Application::GetGameWidth();
		height = (float)Application::GetGameHeight();
	}
	else
	{
		x = (float)Application::GetSceneX();
		y = (float)Application::GetSceneY();
		width = (float)Application::GetSceneWidth();
		height = (float)Application::GetSceneHeight();
	}

	{
		static D3D11_VIEWPORT viewport{};
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = x;
		viewport.TopLeftY = y;
		viewport.Width = width;
		viewport.Height = height;

		Direct3D::DeviceContext()->RSSetViewports(1, &viewport);
	}

	{
		static PassBuffer buffer{};
		buffer.width = (int)width;
		buffer.height = (int)height;

		mPassBuffer.Bind(buffer);
	}

	Direct3D::DeviceContext()->IASetInputLayout(mInputLayoutPass);
	Direct3D::DeviceContext()->VSSetShader(mVertexShaderPass, 0, 0);
	Direct3D::DeviceContext()->PSSetShader(mPixelShaderPass, 0, 0);

	Direct3D::DeviceContext()->VSSetConstantBuffers(0, 1, mPassBuffer.Get());
}

void PassMap::UnbindRTV()
{
	return;
}

void PassMap::Bind(Mesh& mesh)
{
	Matrix world = mesh.GetWorldMatrix();
	Matrix view = Camera::GetViewMatrix();
	Matrix proj = Camera::GetProjMatrix();

	static PrePassBuffer buffer{};
	buffer.worldViewProj = world * view * proj;
	buffer.world = world;

	mPrePassBuffer.Bind(buffer);
}

void PassMap::UpdateShader()
{
	SAFE_RELEASE(mVertexShaderPrePass);
	SAFE_RELEASE(mInputLayoutPrePass);
	SAFE_RELEASE(mPixelShaderPrePass);
	SAFE_RELEASE(mVertexShaderPass);
	SAFE_RELEASE(mInputLayoutPass);
	SAFE_RELEASE(mPixelShaderPass);

	String prePass;
	PathHelper::GetAssetsPath(prePass);
	prePass += L"PrePass.hlsl";

	String pass;
	PathHelper::GetAssetsPath(pass);
	pass += L"Pass.hlsl";

	ID3DBlob* blob = 0;

	{
		CompileShaderFromFile(prePass, "VS", "vs_5_0", &blob);
		FOG_TRACE(Direct3D::Device()->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), 0, &mVertexShaderPrePass));
	}
	{
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		FOG_TRACE(Direct3D::Device()->CreateInputLayout(layout, ARRAYSIZE(layout), blob->GetBufferPointer(), blob->GetBufferSize(), &mInputLayoutPrePass));
	}
	SAFE_RELEASE(blob);
	{
		CompileShaderFromFile(prePass, "PS", "ps_5_0", &blob);
		FOG_TRACE(Direct3D::Device()->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), 0, &mPixelShaderPrePass));
	}
	SAFE_RELEASE(blob);

	{
		CompileShaderFromFile(pass, "VS", "vs_5_0", &blob);
		FOG_TRACE(Direct3D::Device()->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), 0, &mVertexShaderPass));
	}
	{
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		FOG_TRACE(Direct3D::Device()->CreateInputLayout(layout, ARRAYSIZE(layout), blob->GetBufferPointer(), blob->GetBufferSize(), &mInputLayoutPass));
	}
	SAFE_RELEASE(blob);
	{
		CompileShaderFromFile(pass, "PS", "ps_5_0", &blob);
		FOG_TRACE(Direct3D::Device()->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), 0, &mPixelShaderPass));
	}
	SAFE_RELEASE(blob);
}