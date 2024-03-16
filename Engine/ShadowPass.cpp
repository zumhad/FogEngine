#pragma warning(disable : 6387)

#include "ShadowPass.h"

#include "Application.h"
#include "Camera.h"
#include "Matrix.h"
#include "ConstantBuffer.h"
#include "Model.h"
#include "MathHelper.h"
#include "ObjectManager.h"
#include "Light.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "InputLayout.h"

struct ShadowPass::Buffer0
{
	Matrix viewProj;
};

struct ShadowPass::Buffer1
{
	Matrix world;
};

decltype(ShadowPass::mCascade) ShadowPass::mCascade{};

Array<ID3D11DepthStencilView*> ShadowPass::mDepthDSV;

ID3D11ShaderResourceView* ShadowPass::mDepthSRV = 0;

VertexShader ShadowPass::mVertexShader;
InputLayout ShadowPass::mInputLayout;

ConstantBuffer<ShadowPass::Buffer0> ShadowPass::mBuffer0;
ConstantBuffer<ShadowPass::Buffer1> ShadowPass::mBuffer1;

Frustum ShadowPass::mFrustum;

void ShadowPass::Setup()
{
	mCascade.resolution = 1024;
	mCascade.splits.Resize(MAX_CASCADES);
	mCascade.offsets.Resize(MAX_CASCADES);
	mCascade.scales.Resize(MAX_CASCADES);
	mCascade.matrices.Resize(MAX_CASCADES);

	float len = Camera::GetFar() - Camera::GetNear();

	for (int i = 0; i < MAX_CASCADES; i++)
	{
		mCascade.splits[i] = (1.0f / (float)MAX_CASCADES) * (float)(i + 1) * len;
	}

	mDepthDSV.Resize(MAX_CASCADES);

	UpdateTexture();

	mBuffer0.Create();
	mBuffer1.Create();

	{
		mVertexShader.Create(L"ShadowPass.hlsl");

		mInputLayout.Add(L"POSITION");
		mInputLayout.Create(mVertexShader.GetBlob());
	}
}

void ShadowPass::UpdateTexture()
{
	SAFE_RELEASE(mDepthSRV);

	for (int i = 0; i < MAX_CASCADES; i++)
	{
		SAFE_RELEASE(mDepthDSV[i]);
	}

	ID3D11Texture2D* texture = 0;
	{
		D3D11_TEXTURE2D_DESC desc{};
		desc.Width = mCascade.resolution;
		desc.Height = mCascade.resolution;
		desc.MipLevels = 1;
		desc.ArraySize = MAX_CASCADES;
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

		for (int i = 0; i < MAX_CASCADES; i++)
		{
			desc.Texture2DArray.FirstArraySlice = i;
			FOG_TRACE(Direct3D::Device()->CreateDepthStencilView(texture, &desc, &mDepthDSV[i]));
		}
	}
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc{};
		desc.Format = DXGI_FORMAT_R16_UNORM;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
		desc.Texture2D.MostDetailedMip = 0;
		desc.Texture2D.MipLevels = 1;
		desc.Texture2DArray.ArraySize = MAX_CASCADES;
		desc.Texture2DArray.FirstArraySlice = 0;
		FOG_TRACE(Direct3D::Device()->CreateShaderResourceView(texture, &desc, &mDepthSRV));
	}
	SAFE_RELEASE(texture);
}

void ShadowPass::Clear()
{
	for (int i = 0; i < MAX_CASCADES; i++)
	{
		Direct3D::DeviceContext()->ClearDepthStencilView(mDepthDSV[i], D3D11_CLEAR_DEPTH, 0.0f, 0);
	}
}

void ShadowPass::CreateOffsetsAndScales(int index)
{
	Matrix texScaleBias;
	texScaleBias.v[0] = Vector4(0.5f, 0.0f, 0.0f, 0.0f);
	texScaleBias.v[1] = Vector4(0.0f, -0.5f, 0.0f, 0.0f);
	texScaleBias.v[2] = Vector4(0.0f, 0.0f, 1.0f, 0.0f);
	texScaleBias.v[3] = Vector4(0.5f, 0.5f, 0.0f, 1.0f);

	Matrix viewProj = mCascade.matrices[index];
	viewProj = viewProj * texScaleBias;

	Matrix invCascadeMat = Matrix::Inverse(viewProj);
	Vector3 cascadeCorner = Vector3::Transform(Vector3(0.0f, 0.0f, 0.0f), invCascadeMat);
	cascadeCorner = Vector3::Transform(cascadeCorner, mCascade.matrix);

	Vector3 otherCorner = Vector3::Transform(Vector3(1.0f, 1.0f, 1.0f), invCascadeMat);
	otherCorner = Vector3::Transform(otherCorner, mCascade.matrix);

	Vector3 cascadeScale = Vector3(1.0f, 1.0f, 1.0f) / (otherCorner - cascadeCorner);
	mCascade.offsets[index] = Vector4(-cascadeCorner.x, -cascadeCorner.y, -cascadeCorner.z, 0.0f);
	mCascade.scales[index] = Vector4(cascadeScale.x, cascadeScale.y, cascadeScale.z, 1.0f);
}

Matrix ShadowPass::CreateSplits(const Vector3& dir)
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
		float dist = Vector3::Lenght(corner[i] - center);
		sphereRadius = Math::Max(sphereRadius, dist);
	}
	sphereRadius = Math::Ceil(sphereRadius * 16.0f) / 16.0f;

	Vector3 maxExtents = Vector3(sphereRadius, sphereRadius, sphereRadius);
	Vector3 minExtents = -maxExtents;
	Vector3 cascadeExtents = maxExtents - minExtents;

	Matrix proj = DirectX::XMMatrixOrthographicOffCenterLH(minExtents.x, maxExtents.x, minExtents.y, maxExtents.y, 0.0f, cascadeExtents.z);

	Vector3 rot;
	rot.x = -Math::ASin(dir.y);
	rot.y = Math::ATan2(dir.x, dir.z);
	Quaternion q = Quaternion::Euler(rot);
	Vector3 up = Vector3::Rotate(Vector3(0.0f, 1.0f, 0.0f), q);

	Vector3 pos = center + dir * -minExtents.z;

	Matrix view = DirectX::XMMatrixLookAtLH(pos, center, up);

	Vector3 shadowOrigin = -Camera::GetOffsetPosition();
	shadowOrigin = Vector3::Transform(shadowOrigin, view * proj);
	shadowOrigin *= mCascade.resolution * 0.5f;

	Vector3 roundedOrigin = Vector3::Round(shadowOrigin);
	Vector3 roundOffset = roundedOrigin - shadowOrigin;
	roundOffset *= 2.0f / mCascade.resolution;
	roundOffset.z = 0.0f;

	proj.v[3] += Vector4(roundOffset, 0.0f);

	return view * proj;
}

void ShadowPass::CreateMatrix(const Vector3& dir)
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
	Matrix invViewProj = Matrix::Inverse(view * proj);

	Vector3 frustumCenter = 0.0f;
	for (int i = 0; i < 8; ++i)
	{
		frustumCorners[i] = Vector3::Transform(frustumCorners[i], invViewProj);
		frustumCenter += frustumCorners[i];
	}
	frustumCenter /= 8.0f;

	Vector3 rot;
	rot.x = -Math::ASin(dir.y);
	rot.y = Math::ATan2(dir.x, dir.z);
	Quaternion q = Quaternion::Euler(rot);
	Vector3 up = Vector3::Rotate(Vector3(0.0f, 1.0f, 0.0f), q);

	Vector3 pos = frustumCenter + dir * -0.5f;

	view = DirectX::XMMatrixLookAtLH(pos, frustumCenter, up);
	proj = DirectX::XMMatrixOrthographicOffCenterLH(-0.5f, 0.5f, -0.5f, 0.5f, 0.0f, 1.0f);

	Matrix scaling = Matrix::Scaling(0.5f, -0.5f, 1.0f);
	Matrix translation = Matrix::Translation(0.5f, 0.5f, 0.0f);

	mCascade.matrix = (view * proj) * scaling * translation;
}

void ShadowPass::UpdateCascade(const Vector3& dir)
{
	Matrix view = Camera::GetViewMatrix();

	CreateMatrix(dir);

	for (int i = 0; i < MAX_CASCADES; i++)
	{
		float n = Camera::GetNear() + (i ? mCascade.splits[i - 1] : 0.0f);
		float f = Camera::GetNear() + mCascade.splits[i];

		Matrix proj = DirectX::XMMatrixPerspectiveFovLH(Camera::GetFOV(), Camera::GetAspectRatio(), n, f);

		mFrustum.Update(view, proj);
		
		mCascade.matrices[i] = CreateSplits(dir);
		CreateOffsetsAndScales(i);
	}
}

void ShadowPass::Bind()
{
	Vector3 lightDir = Vector3(0.0f, 0.0f, 0.0f);

	int size = ObjectManager::Size<DirectionLight>();
	for (int i = 0; i < size; i++)
	{
		DirectionLight* light = ObjectManager::GetWithNumber<DirectionLight>(i);
		if (!light->GetEnable()) continue;

		lightDir = light->GetDirection();

		break;
	}

	if (lightDir != Vector3(0.0f, 0.0f, 0.0f))
	{
		UpdateViewport();
		Clear();

		UpdateCascade(lightDir);

		Direct3D::DeviceContext()->IASetInputLayout(mInputLayout.Get());
		Direct3D::DeviceContext()->VSSetShader(mVertexShader.Get(), 0, 0);
		Direct3D::DeviceContext()->PSSetShader(0, 0, 0);

		Direct3D::DeviceContext()->VSSetConstantBuffers(0, 1, mBuffer0.Get());
		Direct3D::DeviceContext()->VSSetConstantBuffers(1, 1, mBuffer1.Get());

		for (int i = 0; i < MAX_CASCADES; i++)
		{
			Direct3D::DeviceContext()->OMSetRenderTargets(0, 0, mDepthDSV[i]);

			UpdateBuffer0(i);

			size = ObjectManager::Size<Model>();
			for (int j = 0; j < size; j++)
			{
				Model* model = ObjectManager::GetWithNumber<Model>(j);

				UpdateBuffer1(model);

				model->Draw();
			}
		}
	}
}

void ShadowPass::UpdateViewport()
{
	static D3D11_VIEWPORT viewport{};
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = (float)mCascade.resolution;
	viewport.Height = (float)mCascade.resolution;

	static D3D11_RECT rect{};
	rect.left = 0;
	rect.top = 0;
	rect.right = mCascade.resolution;
	rect.bottom = mCascade.resolution;

	Direct3D::DeviceContext()->RSSetViewports(1, &viewport);
	Direct3D::DeviceContext()->RSSetScissorRects(1, &rect);
}

void ShadowPass::UpdateBuffer0(int index)
{
	static Buffer0 buffer;

	buffer.viewProj = mCascade.matrices[index];

	mBuffer0.Bind(buffer);
}

void ShadowPass::UpdateBuffer1(Model* model)
{
	static Buffer1 buffer;

	buffer.world = model->GetWorldMatrix();

	mBuffer1.Bind(buffer);
}

float ShadowPass::GetBlend()
{
	return mCascade.blend;
}

void ShadowPass::SetBlend(float blend)
{
	FOG_ASSERT(blend >= 0.0f);

	mCascade.blend = blend;
}

void ShadowPass::SetSplit(int index, float split)
{
	FOG_ASSERT(index >= 0 && index < MAX_CASCADES);

	mCascade.splits[index] = split;
}

void ShadowPass::SetBias(float bias)
{
	FOG_ASSERT(bias >= 0.0f);

	mCascade.bias = bias;
}

float ShadowPass::GetBias()
{
	return mCascade.bias;
}

void ShadowPass::SetNormalBias(float bias)
{
	mCascade.normalBias = bias;
}

float ShadowPass::GetNormalBias()
{
	return mCascade.normalBias;
}

float ShadowPass::GetSplit(int index)
{
	FOG_ASSERT(index >= 0 && index < MAX_CASCADES);

	return mCascade.splits[index];
}

Matrix ShadowPass::GetMatrix()
{
	return mCascade.matrix;
}

Vector4 ShadowPass::GetOffset(int index)
{
	FOG_ASSERT(index >= 0 && index < MAX_CASCADES);

	return mCascade.offsets[index];
}

Vector4 ShadowPass::GetScale(int index)
{
	FOG_ASSERT(index >= 0 && index < MAX_CASCADES);

	return mCascade.scales[index];
}

int ShadowPass::GetResolution()
{
	return mCascade.resolution;
}

void ShadowPass::SetResolution(int resolution)
{
	FOG_ASSERT(resolution > 0);

	mCascade.resolution = resolution;
	UpdateTexture();
}

ID3D11DepthStencilView* ShadowPass::GetDepthDSV(int index)
{
	return mDepthDSV[index];
}

ID3D11ShaderResourceView* const* ShadowPass::GetDepthSRV()
{
	return &mDepthSRV;
}

void ShadowPass::Shotdown()
{
	SAFE_RELEASE(mDepthSRV);

	for (int i = 0; i < MAX_CASCADES; i++)
	{
		SAFE_RELEASE(mDepthDSV[i]);
	}

	mBuffer0.Release();
	mBuffer1.Release();
	mVertexShader.Release();
	mInputLayout.Release();
}