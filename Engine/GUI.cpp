#include "GUI.h"

#include "Trace.h"
#include "Shader.h"
#include "CameraEngine.h"

Array<Control*> GUI::mArr;
int GUI::mSize = 0;

ID3D11VertexShader* GUI::mVertexShader = 0;
ID3D11PixelShader* GUI::mPixelShader = 0;
ID3D11InputLayout* GUI::mVertexLayout = 0;
ID3D11Buffer* GUI::mBuffer = 0;

void GUI::Setup()
{
	String shaderPath;
	PathHelper::GetAssetsPath(shaderPath);
	shaderPath += L"GUIShader.hlsl";

	// Vertex Shader
	ID3DBlob* pVSBlob = nullptr;
	CompileShaderFromFile(shaderPath, "VS", "vs_5_0", &pVSBlob);
	FOG_TRACE(Direct3D::Device()->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), 0, &mVertexShader));

	// Input Layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	FOG_TRACE(Direct3D::Device()->CreateInputLayout(layout, ARRAYSIZE(layout), pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &mVertexLayout));
	SAFE_RELEASE(pVSBlob);


	// Pixel Shader
	ID3DBlob* pPSBlob = nullptr;
	CompileShaderFromFile(shaderPath, "PS", "ps_5_0", &pPSBlob);
	FOG_TRACE(Direct3D::Device()->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &mPixelShader));
	SAFE_RELEASE(pPSBlob);

	D3D11_BUFFER_DESC bd = {0};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	FOG_TRACE(Direct3D::Device()->CreateBuffer(&bd, nullptr, &mBuffer));
}

void GUI::Draw()
{
	CameraEngine::Set3D(false);
	Direct3D::SetZBuffer(false);

	Direct3D::DeviceContext()->IASetInputLayout(mVertexLayout);
	Direct3D::DeviceContext()->VSSetShader(mVertexShader, 0, 0);
	Direct3D::DeviceContext()->PSSetShader(mPixelShader, 0, 0);

	for (int i = 0; i < mSize; i++)
	{
		static ConstantBuffer buffer = {};
		buffer.worldViewProj = ((Button*)mArr[i])->GetWorldMatrix() * CameraEngine::GetWorldMatrix() * CameraEngine::GetProjMatrix();
		buffer.material = ((Button*)mArr[i])->color;

		Direct3D::DeviceContext()->UpdateSubresource(mBuffer, 0, 0, &buffer, 0, 0);
		Direct3D::DeviceContext()->VSSetConstantBuffers(0, 1, &mBuffer);
		Direct3D::DeviceContext()->PSSetConstantBuffers(0, 1, &mBuffer);

		((Button*)mArr[i])->Bind();
	}

	Direct3D::SetZBuffer(true);
	CameraEngine::Set3D(true);
}

bool GUI::ClickTest(int x, int y)
{
	for (int i = 0; i < mSize; i++)
	{
		bool isClick =
			(x >= ((Button*)mArr[i])->x) *
			(x <= (((Button*)mArr[i])->x + ((Button*)mArr[i])->width)) *
			(y >= ((Button*)mArr[i])->y) *
			(y <= (((Button*)mArr[i])->y + ((Button*)mArr[i])->height));

		if (isClick) return true;
	}

	return false;
}

void GUI::Click(int x, int y)
{
	for (int i = 0; i < mSize; i++)
	{
		bool isClick =
			(x >= ((Button*)mArr[i])->x) *
			(x <= (((Button*)mArr[i])->x + ((Button*)mArr[i])->width)) *
			(y >= ((Button*)mArr[i])->y) *
			(y <= (((Button*)mArr[i])->y + ((Button*)mArr[i])->height));

		if (isClick)
		{
			((Button*)mArr[i])->Action();
			return;
		}
	}
}

void GUI::Shotdown()
{
	SAFE_RELEASE(mVertexShader);
	SAFE_RELEASE(mPixelShader);
	SAFE_RELEASE(mVertexLayout);
	SAFE_RELEASE(mBuffer);

	for (int i = 0; i < mSize; i++)
	{
		if (mArr[i]->GetType() == TypeControl::Control) delete (Control*)mArr[i];
		if (mArr[i]->GetType() == TypeControl::Button) delete (Button*)mArr[i];
	}
}