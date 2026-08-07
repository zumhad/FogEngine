#include "DebugPass.h"

#include "PixelShader.h"
#include "VertexShader.h"
#include "InputLayout.h"
#include "Direct3D.h"
#include "Camera.h"
#include "ObjectManager.h"
#include "Matrix.h"
#include "ConstantBuffer.h"
#include "Light.h"
#include "Model.h"

struct DebugPass::Buffer0
{
	Matrix viewProj;
};

struct DebugPass::Buffer1
{
	Matrix world;
};

VertexShader DebugPass::mVertexShader;
PixelShader DebugPass::mPixelShader;
InputLayout DebugPass::mInputLayout;

ConstantBuffer<DebugPass::Buffer0> DebugPass::mBuffer0;
ConstantBuffer<DebugPass::Buffer1> DebugPass::mBuffer1;

void DebugPass::Setup()
{
	{
		mVertexShader.Create(L"DebugPass.hlsl", L"VS");
		mPixelShader.Create(L"DebugPass.hlsl", L"PS");

		mInputLayout.Add(L"POSITION");
		mInputLayout.Create(mVertexShader.GetBlob());
	}

	mBuffer0.Create();
	mBuffer1.Create();
}

void DebugPass::Bind()
{
	Direct3D::DeviceContext()->OMSetRenderTargets(1, Direct3D::GetRTV(), 0);

	Direct3D::DeviceContext()->IASetInputLayout(mInputLayout.Get());
	Direct3D::DeviceContext()->VSSetShader(mVertexShader.Get(), 0, 0);
	Direct3D::DeviceContext()->PSSetShader(mPixelShader.Get(), 0, 0);

	Direct3D::DeviceContext()->VSSetConstantBuffers(0, 1, mBuffer0.Get());
	Direct3D::DeviceContext()->VSSetConstantBuffers(1, 1, mBuffer1.Get());

	UpdateBuffer0();

	int size = ObjectManager::Size<DirectionLight>();
	for (int i = 0; i < size; i++)
	{
		Model* model = ObjectManager::GetWithNumber<DirectionLight>(i)->GetModel();

		UpdateBuffer1(model);

		model->BindBoundingBox();
	}

	size = ObjectManager::Size<PointLight>();
	for (int i = 0; i < size; i++)
	{
		Model* model = ObjectManager::GetWithNumber<PointLight>(i)->GetModel();

		UpdateBuffer1(model);

		model->BindBoundingBox();
	}

	size = ObjectManager::Size<Model>();
	for (int i = 0; i < size; i++)
	{
		Model* model = ObjectManager::GetWithNumber<Model>(i);

		UpdateBuffer1(model);

		model->BindBoundingBox();
	}
}

void DebugPass::UpdateBuffer0()
{
	static Buffer0 buffer;
	buffer.viewProj = Camera::GetViewMatrix() * Camera::GetProjMatrix();

	mBuffer0.Bind(buffer);
}

void DebugPass::UpdateBuffer1(Model* model)
{
	static Buffer1 buffer;
	buffer.world = model->GetWorldMatrix();

	mBuffer1.Bind(buffer);
}

void DebugPass::Clear()
{
	static const float color[4]{ 0.0f, 0.0f, 0.0f, 0.0f };

	Direct3D::DeviceContext()->ClearRenderTargetView(*Direct3D::GetRTV(), color);
}

void DebugPass::Shotdown()
{
	mVertexShader.Release();
	mPixelShader.Release();
	mInputLayout.Release();
	mBuffer0.Release();
	mBuffer1.Release();
}