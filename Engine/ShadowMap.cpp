#pragma warning(disable : 6387)

#include "ShadowMap.h"

#include "Application.h"
#include "Camera.h"
#include "Matrix.h"
#include "ConstantBuffer.h"
#include "Model.h"
#include "MathHelper.h"

using namespace DirectX;

struct ShadowMap::ShadowBuffer
{
	Matrix worldViewProj;
};

decltype(ShadowMap::mCascade) ShadowMap::mCascade{};

Array<ID3D11DepthStencilView*> ShadowMap::mDepthStencilView;
ID3D11ShaderResourceView* ShadowMap::mShaderResourceView = 0;
ConstantBuffer<ShadowMap::ShadowBuffer> ShadowMap::mShadowBuffer;
Frustum ShadowMap::mFrustum;

void ShadowMap::Setup()
{
	mCascade.resolution = 1024;
	mCascade.splits.Resize(4);
	mCascade.offsets.Resize(4);
	mCascade.scales.Resize(4);
	mCascade.matrices.Resize(4);

	for (int i = 0; i < 4; i++)
	{
		mCascade.splits[i] = (1.0f / 4.0f) * (i + 1);
	}

	mDepthStencilView.Resize(4);

	UpdateTexture();

	mShadowBuffer.Create();
}

void ShadowMap::UpdateTexture()
{
	SAFE_RELEASE(mShaderResourceView);

	for (int i = 0; i < 4; i++) // size
	{
		SAFE_RELEASE(mDepthStencilView[i]);
	}

	ID3D11Texture2D* texture = 0;
	{
		D3D11_TEXTURE2D_DESC desc{};
		desc.Width = mCascade.resolution;
		desc.Height = mCascade.resolution;
		desc.MipLevels = 1;
		desc.ArraySize = 4; // size
		desc.Format = DXGI_FORMAT_R16_TYPELESS;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		FOG_TRACE(Direct3D::Device()->CreateTexture2D(&desc, 0, &texture));
	}
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC desc{};
		desc.Format = DXGI_FORMAT_D16_UNORM;
		desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
		desc.Texture2DArray.ArraySize = 1;

		for (int i = 0; i < 4; i++) // size
		{
			desc.Texture2DArray.FirstArraySlice = i;
			FOG_TRACE(Direct3D::Device()->CreateDepthStencilView(texture, &desc, &mDepthStencilView[i]));
		}
	}
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc{};
		desc.Format = DXGI_FORMAT_R16_UNORM;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
		desc.Texture2D.MostDetailedMip = 0;
		desc.Texture2D.MipLevels = 1;
		desc.Texture2DArray.ArraySize = 4; // size
		desc.Texture2DArray.FirstArraySlice = 0;
		FOG_TRACE(Direct3D::Device()->CreateShaderResourceView(texture, &desc, &mShaderResourceView));
	}
	SAFE_RELEASE(texture);
}

void ShadowMap::Clear(int index)
{
	Direct3D::DeviceContext()->ClearDepthStencilView(mDepthStencilView[index], D3D11_CLEAR_DEPTH, 0.0f, 0);
}

void ShadowMap::CreateOffsetsAndScales(int index)
{
	XMMATRIX texScaleBias;
	texScaleBias.r[0] = XMVectorSet(0.5f, 0.0f, 0.0f, 0.0f);
	texScaleBias.r[1] = XMVectorSet(0.0f, -0.5f, 0.0f, 0.0f);
	texScaleBias.r[2] = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	texScaleBias.r[3] = XMVectorSet(0.5f, 0.5f, 0.0f, 1.0f);

	Matrix viewProj = mCascade.matrices[index];
	viewProj = XMMatrixMultiply(viewProj, texScaleBias);

	Matrix invCascadeMat = XMMatrixInverse(0, viewProj);
	Vector3 cascadeCorner = XMVector3Transform(Vector3(0.0f, 0.0f, 0.0f), invCascadeMat);
	cascadeCorner = XMVector3Transform(cascadeCorner, mCascade.matrix);

	Vector3 otherCorner = XMVector3Transform(Vector3(1.0f, 1.0f, 1.0f), invCascadeMat);
	otherCorner = XMVector3Transform(otherCorner, mCascade.matrix);

	Vector3 cascadeScale = Vector3(1.0f, 1.0f, 1.0f) / (otherCorner - cascadeCorner);
	mCascade.offsets[index] = Vector4(-cascadeCorner.x, -cascadeCorner.y, -cascadeCorner.z, 0.0f);
	mCascade.scales[index] = Vector4(cascadeScale.x, cascadeScale.y, cascadeScale.z, 1.0f);
}

Matrix ShadowMap::CreateSplits(Vector3 dir)
{
	Vector3 corner[8];
	mFrustum.GetCorners(corner);

	Vector3 center;
	for (int i = 0; i < 8; i++)
	{
		center += corner[i];
	}
	center /= 8.0f;

	float sphereRadius = 0.0f;
	for (int i = 0; i < 8; ++i)
	{
		float dist = XMVectorGetX(XMVector3Length(corner[i] - center));
		sphereRadius = Math::Max(sphereRadius, dist);
	}
	sphereRadius = Math::Ceil(sphereRadius * 16.0f) / 16.0f;

	Vector3 maxExtents = Vector3(sphereRadius, sphereRadius, sphereRadius);
	Vector3 minExtents = -maxExtents;
	Vector3 cascadeExtents = maxExtents - minExtents;

	Matrix proj = XMMatrixOrthographicOffCenterLH(minExtents.x, maxExtents.x, minExtents.y, maxExtents.y, 0.0f, cascadeExtents.z);

	Vector3 pos = center + dir * -minExtents.z;
	Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
	Matrix view = XMMatrixLookAtLH(pos, center, up);

	Vector3 shadowOrigin = Vector3(0.0f, 0.0f, 0.0f);
	shadowOrigin = XMVector4Transform(shadowOrigin, view * proj);
	shadowOrigin = XMVectorScale(shadowOrigin, mCascade.resolution * 0.5f);

	Vector3 roundedOrigin = XMVectorRound(shadowOrigin);
	Vector3 roundOffset = XMVectorSubtract(roundedOrigin, shadowOrigin);
	roundOffset = XMVectorScale(roundOffset, 2.0f / mCascade.resolution);
	roundOffset = XMVectorSetZ(roundOffset, 0.0f);
	roundOffset = XMVectorSetW(roundOffset, 0.0f);

	proj.m[3][0] += roundOffset.x;
	proj.m[3][1] += roundOffset.y;
	proj.m[3][2] += roundOffset.z;

	return view * proj;
}

void ShadowMap::CreateMatrix(Vector3 dir)
{
	Vector3 frustumCorners[8] =
	{
		Vector3(-1.0f,  1.0f, 0.0f),
		Vector3(1.0f,  1.0f, 0.0f),
		Vector3(1.0f, -1.0f, 0.0f),
		Vector3(-1.0f, -1.0f, 0.0f),
		Vector3(-1.0f,  1.0f, 1.0f),
		Vector3(1.0f,  1.0f, 1.0f),
		Vector3(1.0f, -1.0f, 1.0f),
		Vector3(-1.0f, -1.0f, 1.0f)
	};

	Matrix view = Camera::GetViewMatrix();
	Matrix proj = Camera::GetProjMatrix();
	Matrix invViewProj = XMMatrixInverse(0, view * proj);

	Vector3 frustumCenter = 0.0f;
	for (int i = 0; i < 8; ++i)
	{
		frustumCorners[i] = XMVector3Transform(frustumCorners[i], invViewProj);
		frustumCenter += frustumCorners[i];
	}
	frustumCenter /= 8.0f;

	Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
	Vector3 pos = frustumCenter + dir * -0.5f;

	view = XMMatrixLookAtLH(pos, frustumCenter, up);
	proj = XMMatrixOrthographicOffCenterLH(-0.5f, 0.5f, -0.5f, 0.5f, 0.0f, 1.0f);

	Matrix scaling = XMMatrixScaling(0.5f, -0.5f, 1.0f);
	Matrix translation = XMMatrixTranslation(0.5f, 0.5f, 0.0f);

	mCascade.matrix = (view * proj) * scaling * translation;
}

void ShadowMap::UpdateCascade(Vector3 dir)
{
	Matrix view = Camera::GetViewMatrix();

	float len = Camera::GetFar() - Camera::GetNear();

	CreateMatrix(dir);

	for (int i = 0; i < 4; i++)
	{
		float n = Camera::GetNear() + (i ? mCascade.splits[i - 1] * len : 0);
		float f = Camera::GetNear() + mCascade.splits[i] * len;

		Matrix proj = XMMatrixPerspectiveFovLH(Camera::GetFOV(), Camera::GetAspectRatio(), n, f);

		mFrustum.Update(view, proj);
		
		mCascade.matrices[i] = CreateSplits(dir);
		CreateOffsetsAndScales(i);
	}
}

void ShadowMap::UpdateBuffer(Model& model, int index)
{
	static ShadowBuffer buffer;

	buffer.worldViewProj = model.GetWorldMatrix() * mCascade.matrices[index];

	mShadowBuffer.Bind(buffer);
}

void ShadowMap::SetSplit(int index, float split)
{
	FOG_ASSERT(split > 0.0f && split <= 1.0f);

	mCascade.splits[index] = split;
}

float ShadowMap::GetSplit(int index)
{
	FOG_ASSERT(index >= 0 && index < 4);

	float len = Camera::GetFar() - Camera::GetNear();
	return Camera::GetNear() + mCascade.splits[index] * len;
}

Matrix ShadowMap::GetMatrix()
{
	return mCascade.matrix;
}

Matrix ShadowMap::GetMatrix(int index)
{
	FOG_ASSERT(index >= 0 && index < 4);

	return mCascade.matrices[index];
}

Vector4 ShadowMap::GetOffset(int index)
{
	FOG_ASSERT(index >= 0 && index < 4);

	return mCascade.offsets[index];
}

Vector4 ShadowMap::GetScale(int index)
{
	FOG_ASSERT(index >= 0 && index < 4);

	return mCascade.scales[index];
}

int ShadowMap::GetResolution()
{
	return mCascade.resolution;
}

void ShadowMap::SetResolution(int resolution)
{
	FOG_ASSERT(resolution > 0);

	mCascade.resolution = resolution;
	UpdateTexture();
}

ID3D11Buffer* const* ShadowMap::GetBuffer()
{
	return mShadowBuffer.Get();
}

ID3D11DepthStencilView* ShadowMap::GetDSV(int index)
{
	return mDepthStencilView[index];
}

ID3D11ShaderResourceView* const* ShadowMap::GetSRV()
{
	return &mShaderResourceView;
}

void ShadowMap::Shotdown()
{
	SAFE_RELEASE(mShaderResourceView);

	for (int i = 0; i < 4; i++) // size
	{
		SAFE_RELEASE(mDepthStencilView[i]);
	}

	mShadowBuffer.Release();
}