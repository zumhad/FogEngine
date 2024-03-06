#include "OutlineMap.h"

#include "Utility.h"
#include "Direct3D.h"
#include "Application.h"
#include "SamplerState.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "InputLayout.h"
#include "CustomArray.h"
#include "TextureMap.h"
#include "ConstantBuffer.h"
#include "LightMap.h"
#include "ObjectManager.h"
#include "Object.h"
#include "Model.h"
#include "Camera.h"
#include "Light.h"
#include "MathHelper.h"
#include "ComputeShader.h"

decltype(OutlineMap::mOutline) OutlineMap::mOutline{};

VertexShader OutlineMap::mInitVS;
PixelShader OutlineMap::mInitPS;
InputLayout OutlineMap::mInitIL;
ComputeShader OutlineMap::mPrePassCS;
VertexShader OutlineMap::mPassVS;
PixelShader OutlineMap::mPassPS;
InputLayout OutlineMap::mPassIL;

ID3D11RenderTargetView* OutlineMap::mRenderTargetView = 0;
ID3D11ShaderResourceView* OutlineMap::mShaderResourceView = 0;
ID3D11UnorderedAccessView* OutlineMap::mUnorderedAccessView = 0;

struct OutlineMap::OutlineInitBuffer0
{
	Matrix viewProj;
};
ConstantBuffer<OutlineMap::OutlineInitBuffer0> OutlineMap::mOutlineInitBuffer0;

struct OutlineMap::OutlineInitBuffer1
{
	Matrix world;
};
ConstantBuffer<OutlineMap::OutlineInitBuffer1> OutlineMap::mOutlineInitBuffer1;

struct OutlineMap::OutlinePrePassBuffer
{
	int stepSize;
	int width;
	int height; float pad;
};
ConstantBuffer<OutlineMap::OutlinePrePassBuffer> OutlineMap::mOutlinePrePassBuffer;

struct OutlineMap::OutlinePassBuffer
{
	Color color;
	int stepSize;
	int width;
	int height; float pad;
};
ConstantBuffer<OutlineMap::OutlinePassBuffer> OutlineMap::mOutlinePassBuffer;

void OutlineMap::Setup()
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
		desc.Format = DXGI_FORMAT_R16G16_SINT;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		FOG_TRACE(Direct3D::Device()->CreateTexture2D(&desc, 0, &texture));
	}
	{
		D3D11_RENDER_TARGET_VIEW_DESC desc{};
		desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		desc.Format = DXGI_FORMAT_R16G16_SINT;
		FOG_TRACE(Direct3D::Device()->CreateRenderTargetView(texture, &desc, &mRenderTargetView));
	}
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc{};
		desc.Format = DXGI_FORMAT_R16G16_SINT;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MostDetailedMip = 0;
		desc.Texture2D.MipLevels = 1;
		FOG_TRACE(Direct3D::Device()->CreateShaderResourceView(texture, &desc, &mShaderResourceView));
	}
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC desc{};
		desc.Format = DXGI_FORMAT_R16G16_SINT;
		desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipSlice = 0;
		Direct3D::Device()->CreateUnorderedAccessView(texture, &desc, &mUnorderedAccessView);
	}
	SAFE_RELEASE(texture);

	{
		mInitVS.Create(L"OutlineInit.hlsl");
		mInitPS.Create(L"OutlineInit.hlsl");

		Array<String> input;
		input.Add(L"POSITION");
		mInitIL.Create(mInitVS.GetBlob(), input);
	}
	{
		mPassVS.Create(L"OutlinePass.hlsl");
		mPassPS.Create(L"OutlinePass.hlsl");

		Array<String> input;
		input.Add(L"TEXCOORD");
		mPassIL.Create(mPassVS.GetBlob(), input);
	}
	{
		mPrePassCS.Create("OutlinePrePass.hlsl");
	}

	mOutlineInitBuffer0.Create();
	mOutlineInitBuffer1.Create();
	mOutlinePrePassBuffer.Create();
	mOutlinePassBuffer.Create();
}

void OutlineMap::Bind()
{
	bool isDrawOutline = false;

	Direct3D::DeviceContext()->IASetInputLayout(mInitIL.Get());
	Direct3D::DeviceContext()->VSSetShader(mInitVS.Get(), 0, 0);
	Direct3D::DeviceContext()->PSSetShader(mInitPS.Get(), 0, 0);

	Direct3D::DeviceContext()->OMSetRenderTargets(1, &mRenderTargetView, 0);

	UpdateInitBuffer0();

	int size = ObjectManager::Size<Model>();
	for (int i = 0; i < size; i++)
	{
		Model* model = ObjectManager::GetWithNumber<Model>(i);

		if (model->outline)
		{
			UpdateInitBuffer1(model);

			model->Draw();

			isDrawOutline = true;
		}
	}

	size = ObjectManager::Size<PointLight>();
	for (int i = 0; i < size; i++)
	{
		PointLight* light = ObjectManager::GetWithNumber<PointLight>(i);

		if (light->outline)
		{
			UpdateInitBuffer1(light->GetModel());

			light->Bind();

			isDrawOutline = true;
		}
	}

	size = ObjectManager::Size<DirectionLight>();
	for (int i = 0; i < size; i++)
	{
		DirectionLight* light = ObjectManager::GetWithNumber<DirectionLight>(i);

		if (light->outline)
		{
			UpdateInitBuffer1(light->GetModel());

			light->Bind();

			isDrawOutline = true;
		}
	}

	Direct3D::DeviceContext()->OMSetRenderTargets(1, Direct3D::NullRTV(), 0);

	if (!isDrawOutline) return;

	//////////////////////////////////////////////////////////////////////////

	Direct3D::DeviceContext()->CSSetShader(mPrePassCS.Get(), 0, 0);
	Direct3D::DeviceContext()->CSSetUnorderedAccessViews(0, 1, &mUnorderedAccessView, 0);

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

	int threadX = Math::Ceil(width / 32.0f);
	int threadY = Math::Ceil(height / 32.0f);

	int it = Math::Ceil(Math::Log2(mOutline.width + 1.0f));
	for (int i = 0; i < it; i++)
	{
		UpdatePrePassBuffer((int)Math::Pow(2.0f, (float)it - i - 1.0f));
		Direct3D::DeviceContext()->CSSetConstantBuffers(0, 1, mOutlinePrePassBuffer.Get());

		Direct3D::DeviceContext()->Dispatch(threadX, threadY, 1);
	}

	Direct3D::DeviceContext()->CSSetUnorderedAccessViews(0, 1, Direct3D::NullUAV(), 0);

	////////////////////////////////////////////////////////////////////////////////

	Direct3D::DeviceContext()->OMSetRenderTargets(1, LightMap::GetRTV(), 0);

	Direct3D::DeviceContext()->IASetInputLayout(mPassIL.Get());
	Direct3D::DeviceContext()->VSSetShader(mPassVS.Get(), 0, 0);
	Direct3D::DeviceContext()->PSSetShader(mPassPS.Get(), 0, 0);

	UpdatePassBuffer();
	Direct3D::DeviceContext()->PSSetConstantBuffers(0, 1, mOutlinePassBuffer.Get());

	Direct3D::DeviceContext()->PSSetShaderResources(0, 1, &mShaderResourceView);

	TextureMap::Bind();

	Direct3D::DeviceContext()->PSSetShaderResources(0, 1, Direct3D::NullSRV());
}

void OutlineMap::UpdateInitBuffer0()
{
	static OutlineInitBuffer0 buffer{};
	buffer.viewProj = Camera::GetViewMatrix() * Camera::GetProjMatrix();

	mOutlineInitBuffer0.Bind(buffer);
	Direct3D::DeviceContext()->VSSetConstantBuffers(0, 1, mOutlineInitBuffer0.Get());
}

void OutlineMap::UpdateInitBuffer1(Model* obj)
{
	static OutlineInitBuffer1 buffer{};
	buffer.world = obj->GetWorldMatrix();

	mOutlineInitBuffer1.Bind(buffer);
	Direct3D::DeviceContext()->VSSetConstantBuffers(1, 1, mOutlineInitBuffer1.Get());
}

void OutlineMap::UpdatePrePassBuffer(int stepSize)
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

	static OutlinePrePassBuffer buffer{};
	buffer.stepSize = stepSize;
	buffer.width = width;
	buffer.height = height;

	mOutlinePrePassBuffer.Bind(buffer);
}

void OutlineMap::UpdatePassBuffer()
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

	static OutlinePassBuffer buffer{};
	buffer.color = mOutline.color;
	buffer.stepSize = mOutline.width;
	buffer.width = width;
	buffer.height = height;

	mOutlinePassBuffer.Bind(buffer);
}

void OutlineMap::Shotdown()
{
	mInitVS.Release();
	mInitPS.Release();
	mInitIL.Release();
	mPrePassCS.Release();
	mPassVS.Release();
	mPassPS.Release();
	mPassIL.Release();
	mOutlineInitBuffer0.Release();
	mOutlineInitBuffer1.Release();
	mOutlinePrePassBuffer.Release();
	mOutlinePassBuffer.Release();

	SAFE_RELEASE(mUnorderedAccessView);
	SAFE_RELEASE(mRenderTargetView);
	SAFE_RELEASE(mShaderResourceView);
}

void OutlineMap::Clear()
{
	static const float color[4]{ 32767.0f, 32767.0f, 32767.0f, 32767.0f };
	Direct3D::DeviceContext()->ClearRenderTargetView(mRenderTargetView, color);
}

ID3D11RenderTargetView* OutlineMap::GetRTV()
{
	return mRenderTargetView;
}