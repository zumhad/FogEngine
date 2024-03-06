#pragma warning(disable : 6387)

#include "LightMap.h"

#include "Application.h"
#include "Utility.h"
#include "PipelineState.h"
#include "Camera.h"
#include "ShadowMap.h"
#include "Light.h"
#include "Matrix.h"
#include "ConstantBuffer.h"

struct FOG_API LightMap::LightBuffer
{
	struct
	{
		Color color;
		Vector3 dir;
		float power;
	} dirLight{};
	struct
	{
		Color color;
		Vector3 position;
		float radius;
		float power; float pad[3];
	} pointLight[16]{};
	Matrix matrix;
	float shadowSplit[4];
	Vector4 shadowOffset[4];
	Vector4 shadowScale[4];
	Vector3 cameraPos;
	int pointCount = 0;
	int width = 0;
	int height = 0; float pad[2];
};

ID3D11RenderTargetView* LightMap::mRenderTargetView = 0;
ID3D11ShaderResourceView* LightMap::mShaderResourceView = 0;
LightMap::LightBuffer LightMap::mBuffer{};

ConstantBuffer<LightMap::LightBuffer> LightMap::mLightBuffer;

void LightMap::Setup()
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
		desc.Format = DXGI_FORMAT_R16G16B16A16_UNORM;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		FOG_TRACE(Direct3D::Device()->CreateTexture2D(&desc, 0, &texture));
	}
	{
		D3D11_RENDER_TARGET_VIEW_DESC desc{};
		desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		desc.Format = DXGI_FORMAT_R16G16B16A16_UNORM;
		FOG_TRACE(Direct3D::Device()->CreateRenderTargetView(texture, &desc, &mRenderTargetView));
	}
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc{};
		desc.Format = DXGI_FORMAT_R16G16B16A16_UNORM;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MostDetailedMip = 0;
		desc.Texture2D.MipLevels = 1;
		FOG_TRACE(Direct3D::Device()->CreateShaderResourceView(texture, &desc, &mShaderResourceView));
	}
	SAFE_RELEASE(texture);

	mLightBuffer.Create();
}

void LightMap::UpdateBuffer(DirectionLight& dir)
{
	mBuffer.dirLight.color = dir.color;
	mBuffer.dirLight.dir = dir.GetDirection();
	mBuffer.dirLight.power = dir.power;

	for (int i = 0; i < 4; i++)
	{
		mBuffer.shadowSplit[i] = ShadowMap::GetSplit(i);
		mBuffer.shadowOffset[i] = ShadowMap::GetOffset(i);
		mBuffer.shadowScale[i] = ShadowMap::GetScale(i);
		mBuffer.matrix = ShadowMap::GetMatrix();
	}
}

void LightMap::UpdateBuffer(PointLight& point)
{
	mBuffer.pointLight[mBuffer.pointCount].color = point.color;
	mBuffer.pointLight[mBuffer.pointCount].position = point.position;
	mBuffer.pointLight[mBuffer.pointCount].power = point.power;
	mBuffer.pointLight[mBuffer.pointCount].radius = point.radius;
	mBuffer.pointCount++;
}

void LightMap::UpdateBuffer()
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

	mBuffer.width = width;
	mBuffer.height = height;
	mBuffer.cameraPos = Camera::GetPosition();

	mLightBuffer.Bind(mBuffer);

	memset(&mBuffer, 0, sizeof(LightBuffer));
}

void LightMap::Clear()
{
	const float* color = 0;

	if (Application::IsGame())
	{
		color = Application::GetGameColor();
	}
	else
	{
		color = Application::GetSceneColor();
	}

	Direct3D::DeviceContext()->ClearRenderTargetView(mRenderTargetView, color);
}

void LightMap::Shotdown()
{
	SAFE_RELEASE(mRenderTargetView);
	SAFE_RELEASE(mShaderResourceView);

	mLightBuffer.Release();
}

ID3D11ShaderResourceView* const* LightMap::GetSRV()
{
	return &mShaderResourceView;
}

ID3D11RenderTargetView* const* LightMap::GetRTV()
{
	return &mRenderTargetView;
}

ID3D11Buffer* const* LightMap::GetBuffer()
{
	return mLightBuffer.Get();
}