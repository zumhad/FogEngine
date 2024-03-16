#include "OutlinePass.h"

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

decltype(OutlinePass::mOutline) OutlinePass::mOutline{};

VertexShader OutlinePass::mVertexShader0;
PixelShader OutlinePass::mPixelShader0;
InputLayout OutlinePass::mInputLayout0;
ComputeShader OutlinePass::mCopmuteShader;
VertexShader OutlinePass::mVertexShader1;
PixelShader OutlinePass::mPixelShader1;
InputLayout OutlinePass::mInputLayout1;

ID3D11RenderTargetView* OutlinePass::mOffsetRTV = 0;
ID3D11ShaderResourceView* OutlinePass::mOffsetSRV = 0;
ID3D11UnorderedAccessView* OutlinePass::mOffsetUAV = 0;

struct OutlinePass::Buffer0
{
	Matrix viewProj;
};
ConstantBuffer<OutlinePass::Buffer0> OutlinePass::mBuffer0;

struct OutlinePass::Buffer1
{
	Matrix world;
};
ConstantBuffer<OutlinePass::Buffer1> OutlinePass::mBuffer1;

struct OutlinePass::Buffer2
{
	int stepSize;
	int width;
	int height; float pad;
};
ConstantBuffer<OutlinePass::Buffer2> OutlinePass::mBuffer2;

struct OutlinePass::Buffer3
{
	Color color;
	int stepSize;
	int width;
	int height; float pad;
};
ConstantBuffer<OutlinePass::Buffer3> OutlinePass::mBuffer3;

void OutlinePass::Setup()
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
		FOG_TRACE(Direct3D::Device()->CreateRenderTargetView(texture, &desc, &mOffsetRTV));
	}
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc{};
		desc.Format = DXGI_FORMAT_R16G16_SINT;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MostDetailedMip = 0;
		desc.Texture2D.MipLevels = 1;
		FOG_TRACE(Direct3D::Device()->CreateShaderResourceView(texture, &desc, &mOffsetSRV));
	}
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC desc{};
		desc.Format = DXGI_FORMAT_R16G16_SINT;
		desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipSlice = 0;
		Direct3D::Device()->CreateUnorderedAccessView(texture, &desc, &mOffsetUAV);
	}
	SAFE_RELEASE(texture);

	{
		mVertexShader0.Create(L"OutlinePass.hlsl", L"VS0");
		mPixelShader0.Create(L"OutlinePass.hlsl", L"PS0");

		mInputLayout0.Add(L"POSITION");
		mInputLayout0.Create(mVertexShader0.GetBlob());
	}
	{
		mCopmuteShader.Create(L"OutlinePassCS.hlsl");
	}
	{
		mVertexShader1.Create(L"OutlinePass.hlsl", L"VS1");
		mPixelShader1.Create(L"OutlinePass.hlsl", L"PS1");

		mInputLayout1.Add(L"TEXCOORD");
		mInputLayout1.Create(mVertexShader1.GetBlob());
	}

	mBuffer0.Create();
	mBuffer1.Create();
	mBuffer2.Create();
	mBuffer3.Create();
}

void OutlinePass::Bind()
{
	bool isDrawOutline = false;

	Clear();
	UpdateBuffer0();

	Direct3D::DeviceContext()->IASetInputLayout(mInputLayout0.Get());
	Direct3D::DeviceContext()->VSSetShader(mVertexShader0.Get(), 0, 0);
	Direct3D::DeviceContext()->PSSetShader(mPixelShader0.Get(), 0, 0);

	Direct3D::DeviceContext()->OMSetRenderTargets(1, &mOffsetRTV, 0);

	Direct3D::DeviceContext()->VSSetConstantBuffers(0, 1, mBuffer0.Get());
	Direct3D::DeviceContext()->VSSetConstantBuffers(1, 1, mBuffer1.Get());

	int size = ObjectManager::Size<Model>();
	for (int i = 0; i < size; i++)
	{
		Model* model = ObjectManager::GetWithNumber<Model>(i);

		if (model->GetOutline() && model->GetEnable())
		{
			UpdateBuffer1(model);

			model->Draw();

			isDrawOutline = true;
		}
	}

	size = ObjectManager::Size<PointLight>();
	for (int i = 0; i < size; i++)
	{
		PointLight* light = ObjectManager::GetWithNumber<PointLight>(i);

		if (light->GetOutline() && light->GetEnable())
		{
			UpdateBuffer1(light->GetModel());

			light->Bind();

			isDrawOutline = true;
		}
	}

	size = ObjectManager::Size<DirectionLight>();
	for (int i = 0; i < size; i++)
	{
		DirectionLight* light = ObjectManager::GetWithNumber<DirectionLight>(i);

		if (light->GetOutline() && light->GetEnable())
		{
			UpdateBuffer1(light->GetModel());

			light->Bind();

			isDrawOutline = true;
		}
	}

	Direct3D::DeviceContext()->OMSetRenderTargets(1, Direct3D::NullRTV(), 0);

	if (!isDrawOutline) return;

	//////////////////////////////////////////////////////////////////////////

	Direct3D::DeviceContext()->CSSetShader(mCopmuteShader.Get(), 0, 0);
	Direct3D::DeviceContext()->CSSetUnorderedAccessViews(0, 1, &mOffsetUAV, 0);

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
		UpdateBuffer2((int)Math::Pow(2.0f, (float)it - i - 1.0f));
		Direct3D::DeviceContext()->CSSetConstantBuffers(0, 1, mBuffer2.Get());

		Direct3D::DeviceContext()->Dispatch(threadX, threadY, 1);
	}

	Direct3D::DeviceContext()->CSSetUnorderedAccessViews(0, 1, Direct3D::NullUAV(), 0);

	////////////////////////////////////////////////////////////////////////////////

	UpdateBuffer3();

	Direct3D::DeviceContext()->OMSetRenderTargets(1, LightMap::GetRTV(), 0);

	Direct3D::DeviceContext()->IASetInputLayout(mInputLayout1.Get());
	Direct3D::DeviceContext()->VSSetShader(mVertexShader1.Get(), 0, 0);
	Direct3D::DeviceContext()->PSSetShader(mPixelShader1.Get(), 0, 0);

	Direct3D::DeviceContext()->PSSetConstantBuffers(0, 1, mBuffer3.Get());

	Direct3D::DeviceContext()->PSSetShaderResources(0, 1, &mOffsetSRV);

	TextureMap::Bind();

	Direct3D::DeviceContext()->PSSetShaderResources(0, 1, Direct3D::NullSRV());
}

void OutlinePass::UpdateBuffer0()
{
	static Buffer0 buffer{};
	buffer.viewProj = Camera::GetViewMatrix() * Camera::GetProjMatrix();

	mBuffer0.Bind(buffer);
}

void OutlinePass::UpdateBuffer1(Model* obj)
{
	static Buffer1 buffer{};
	buffer.world = obj->GetWorldMatrix();

	mBuffer1.Bind(buffer);
}

void OutlinePass::UpdateBuffer2(int stepSize)
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

	static Buffer2 buffer{};
	buffer.stepSize = stepSize;
	buffer.width = width;
	buffer.height = height;

	mBuffer2.Bind(buffer);
}

void OutlinePass::UpdateBuffer3()
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

	static Buffer3 buffer{};
	buffer.color = mOutline.color;
	buffer.stepSize = mOutline.width;
	buffer.width = width;
	buffer.height = height;

	mBuffer3.Bind(buffer);
}

void OutlinePass::Shotdown()
{
	mVertexShader0.Release();
	mPixelShader0.Release();
	mInputLayout0.Release();
	mCopmuteShader.Release();
	mVertexShader1.Release();
	mPixelShader1.Release();
	mInputLayout1.Release();
	mBuffer0.Release();
	mBuffer1.Release();
	mBuffer2.Release();
	mBuffer3.Release();

	SAFE_RELEASE(mOffsetRTV);
	SAFE_RELEASE(mOffsetSRV);
	SAFE_RELEASE(mOffsetUAV);
}

void OutlinePass::SetColor(const Color& color)
{
	mOutline.color = color;
}

void OutlinePass::SetWidth(int width)
{
	mOutline.width = width;
}

void OutlinePass::Clear()
{
	static const float color[4]{ 32767.0f, 32767.0f, 32767.0f, 32767.0f };
	Direct3D::DeviceContext()->ClearRenderTargetView(mOffsetRTV, color);
}

ID3D11RenderTargetView* const* OutlinePass::GetOffsetRTV()
{
	return &mOffsetRTV;
}

ID3D11ShaderResourceView* const* OutlinePass::GetOffsetSRV()
{
	return &mOffsetSRV;
}

ID3D11UnorderedAccessView* const* OutlinePass::GetOffsetUAV()
{
	return &mOffsetUAV;
}