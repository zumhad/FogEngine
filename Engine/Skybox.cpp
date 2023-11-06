#include "Skybox.h"

#include "Model.h"
#include "Utility.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "InputLayout.h"
#include "Direct3D.h"
#include "ConstantBuffer.h"
#include "Camera.h"

using namespace DirectX;

struct Skybox::SkyboxBuffer
{
	Matrix worldViewProj;
};

Model* Skybox::mModel = 0;
VertexShader Skybox::mVertexShader;
PixelShader Skybox::mPixelShader;
InputLayout Skybox::mInputLayout;
ConstantBuffer<Skybox::SkyboxBuffer> Skybox::mSkyboxBuffer;

void Skybox::Setup()
{
	Model model;
	model.name = L"sphere.obj";
	model.texture = L"skybox.png";

	mModel = new Model(std::move(model));

	{
		mVertexShader.Create(L"Skybox.hlsl");
		mPixelShader.Create(L"Skybox.hlsl");

		Array<String> input;
		input.Add(L"POSITION");
		input.Add(L"NORMAL");
		input.Add(L"TEXCOORD");
		mInputLayout.Create(mVertexShader.GetBlob(), input);
	}

	mSkyboxBuffer.Create();
}

void Skybox::Bind()
{
	Vector3 pos = Camera::GetPosition();

	static Matrix world;
	world.m[3][0] = pos.x;
	world.m[3][1] = pos.y;
	world.m[3][2] = pos.z;

	static SkyboxBuffer buffer{};
	buffer.worldViewProj = world * Camera::GetViewMatrix() * Camera::GetProjMatrix();
	mSkyboxBuffer.Bind(buffer);

	Direct3D::DeviceContext()->IASetInputLayout(mInputLayout.Get());
	Direct3D::DeviceContext()->VSSetShader(mVertexShader.Get(), 0, 0);
	Direct3D::DeviceContext()->PSSetShader(mPixelShader.Get(), 0, 0);

	Direct3D::DeviceContext()->VSSetConstantBuffers(0, 1, mSkyboxBuffer.Get());

	mModel->BindTexture();
	mModel->Draw();
}

void Skybox::Shotdown()
{
	mVertexShader.Release();
	mPixelShader.Release();
	mInputLayout.Release();
	mSkyboxBuffer.Release();

	SAFE_DELETE(mModel);
}