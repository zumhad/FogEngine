#pragma warning(disable : 6387)

#include "LightMap.h"

#include "Application.h"
#include "Utility.h"
#include "PipelineState.h"
#include "Camera.h"
#include "ShadowPass.h"
#include "Light.h"
#include "Matrix.h"
#include "ConstantBuffer.h"

struct FOG_API LightMap::LightBuffer
{
	struct
	{
		Matrix viewProj;
		Vector4 offset[MAX_CASCADES];
		Vector4 scale[MAX_CASCADES];
		Color color;
		Vector3 direction;
		float power;
		float bias; 
		float normalBias;
		float blend; float pad;
	} dirLight{};
	struct
	{
		Color color;
		Vector3 position;
		float radius;
		float power; float pad[3];
	} pointLight[MAX_POINT_LIGHT]{};
	Vector3 cameraPosition;
	int width;
	int height;
	int pointCount; float pad[2];
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

void LightMap::UpdateBuffer(DirectionLight* dir)
{
	mBuffer.dirLight.color = dir->GetColor();
	mBuffer.dirLight.direction = dir->GetDirection();
	mBuffer.dirLight.power = dir->GetPower();
	mBuffer.dirLight.bias = ShadowPass::GetBias();
	mBuffer.dirLight.normalBias = ShadowPass::GetNormalBias();
	mBuffer.dirLight.blend = ShadowPass::GetBlend();

	for (int i = 0; i < MAX_CASCADES; i++)
	{
		mBuffer.dirLight.offset[i] = ShadowPass::GetOffset(i);
		mBuffer.dirLight.scale[i] = ShadowPass::GetScale(i);
		mBuffer.dirLight.viewProj = ShadowPass::GetMatrix();
	}
}

int LightMap::UpdateBuffer(PointLight* point)
{
	mBuffer.pointLight[mBuffer.pointCount].color = point->GetColor();
	mBuffer.pointLight[mBuffer.pointCount].position = point->GetPosition() - Camera::GetOffsetPosition();
	mBuffer.pointLight[mBuffer.pointCount].power = point->GetPower();
	mBuffer.pointLight[mBuffer.pointCount].radius = point->GetRadius();
	mBuffer.pointCount++;

	return mBuffer.pointCount;
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
	mBuffer.cameraPosition = Camera::GetRealPosition();

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