#include "SkyboxPass.h"

#include "Model.h"
#include "Utility.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "InputLayout.h"
#include "Direct3D.h"
#include "ConstantBuffer.h"
#include "Camera.h"

struct SkyboxPass::Buffer0
{
	Matrix worldViewProj;
};

Model* SkyboxPass::mModel = 0;
VertexShader SkyboxPass::mVertexShader;
PixelShader SkyboxPass::mPixelShader;
InputLayout SkyboxPass::mInputLayout;
ConstantBuffer<SkyboxPass::Buffer0> SkyboxPass::mBuffer0;

void SkyboxPass::Setup()
{
	Model model;
	model.SetModelPath(L"sphere.obj");
	model.SetTexturePath(L"skybox.png");

	mModel = new Model(std::move(model));

	{
		mVertexShader.Create(L"SkyboxPass.hlsl");
		mPixelShader.Create(L"SkyboxPass.hlsl");

		mInputLayout.Add(L"POSITION");
		mInputLayout.Add(L"NORMAL");
		mInputLayout.Add(L"TEXCOORD");
		mInputLayout.Create(mVertexShader.GetBlob());
	}

	mBuffer0.Create();
}

void SkyboxPass::Bind()
{
	UpdateBuffer0();

	Direct3D::DeviceContext()->IASetInputLayout(mInputLayout.Get());
	Direct3D::DeviceContext()->VSSetShader(mVertexShader.Get(), 0, 0);
	Direct3D::DeviceContext()->PSSetShader(mPixelShader.Get(), 0, 0);

	Direct3D::DeviceContext()->VSSetConstantBuffers(0, 1, mBuffer0.Get());

	mModel->BindTexture();
	mModel->Draw();
}

void SkyboxPass::UpdateBuffer0()
{
	static Matrix world;
	world.v[3] = Camera::GetRealPosition();

	static Buffer0 buffer{};
	buffer.worldViewProj = world * Camera::GetViewMatrix() * Camera::GetProjMatrix();
	mBuffer0.Bind(buffer);
}

void SkyboxPass::Shotdown()
{
	mVertexShader.Release();
	mPixelShader.Release();
	mInputLayout.Release();
	mBuffer0.Release();

	SAFE_DELETE(mModel);
}