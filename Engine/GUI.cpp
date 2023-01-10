#include "GUI.h"

#include "Application.h"
#include "Trace.h"
#include "Shader.h"
#include "CameraEngine.h"
#include "Cursor.h"
#include "CustomString.h"
#include "PathHelper.h"
#include "InputEngine.h"

using namespace DirectX;
using namespace std;

GUI::Data* GUI::mData = 0;

struct GUI::Data
{
public:
	Data();
	~Data();

public:
	std::vector<Control*> v;
	int size;

	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11InputLayout* vertexLayout;
	ID3D11Buffer* buffer;
};


GUI::Data::Data() : size(0), vertexShader(0), pixelShader(0), vertexLayout(0), buffer(0)
{
	String shaderPath;
	PathHelper::GetAssetsPath(shaderPath);
	shaderPath += L"GUIShader.hlsl";

	// Vertex Shader
	ID3DBlob* pVSBlob = nullptr;
	CompileShaderFromFile(shaderPath, "VS", "vs_5_0", &pVSBlob);
	FOG_TRACE(Direct3D::Device()->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), 0, &vertexShader));

	// Input Layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	FOG_TRACE(Direct3D::Device()->CreateInputLayout(layout, ARRAYSIZE(layout), pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &vertexLayout));
	SAFE_RELEASE(pVSBlob);


	// Pixel Shader
	ID3DBlob* pPSBlob = nullptr;
	CompileShaderFromFile(shaderPath, "PS", "ps_5_0", &pPSBlob);
	FOG_TRACE(Direct3D::Device()->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), 0, &pixelShader));
	SAFE_RELEASE(pPSBlob);

	D3D11_BUFFER_DESC bd = { 0 };
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.ByteWidth = sizeof(ObjectBuffer);
	FOG_TRACE(Direct3D::Device()->CreateBuffer(&bd, 0, &buffer));
}

void GUI::Setup()
{
	mData = new Data;
}

Control& GUI::Get(int i)
{ 
	if (i + 1 > mData->size)
		Application::Exit();

	return *(mData->v[i]);
}

void GUI::Draw()
{
	CameraEngine::Set3D(false);
	Direct3D::SetZBuffer(false);

	Direct3D::DeviceContext()->IASetInputLayout(mData->vertexLayout);
	Direct3D::DeviceContext()->VSSetShader(mData->vertexShader, 0, 0);
	Direct3D::DeviceContext()->PSSetShader(mData->pixelShader, 0, 0);

	static ObjectBuffer buffer = {};

	for (int i = 0; i < mData->size; i++)
	{
		TypeControl type = Get(i).GetType();

		if (type == TypeControl::Button)
		{
			Button& button = (Button&)Get(i);

			buffer.worldViewProj = button.GetWorldMatrix() * CameraEngine::GetViewMatrix() * CameraEngine::GetProjMatrix();
			buffer.material = button.color;

			Direct3D::DeviceContext()->UpdateSubresource(mData->buffer, 0, 0, &buffer, 0, 0);
			Direct3D::DeviceContext()->VSSetConstantBuffers(0, 1, &mData->buffer);
			Direct3D::DeviceContext()->PSSetConstantBuffers(0, 1, &mData->buffer);

			button.Bind();
		}
	}

	Direct3D::SetZBuffer(true);
	CameraEngine::Set3D(true);
}

bool GUI::ClickTest()
{
	int x = Cursor::GetPosition(CURSOR_X);
	int y = Cursor::GetPosition(CURSOR_Y);

	for (int i = 0; i < mData->size; i++)
	{
		Button& b = (Button&)Get(i);
		bool isClick = (x >= b.x) * (x <= b.x + b.width) * (y >= b.y) * (y <= b.y + b.height);

		if (isClick) return true;
	}

	return false;
}

void GUI::Click()
{
	if (!Input::Down(MOUSE_LEFT)) return;

	int x = Cursor::GetPosition(CURSOR_X);
	int y = Cursor::GetPosition(CURSOR_Y);

	for (int i = 0; i < mData->size; i++)
	{
		Button& b = (Button&)Get(i);
		bool isClick = (x >= b.x) * (x <= b.x + b.width) * (y >= b.y) * (y <= b.y + b.height);

		if (isClick)
		{
			b.Action();
			return;
		}
	}
}

int GUI::Size()
{ 
	return mData->size; 
}

void GUI::Shotdown()
{
	SAFE_DELETE(mData);
}

GUI::Data::~Data()
{
	SAFE_RELEASE(vertexShader);
	SAFE_RELEASE(pixelShader);
	SAFE_RELEASE(vertexLayout);
	SAFE_RELEASE(buffer);

	for (int i = 0; i < size; i++)
	{
		TypeControl type = v[i]->GetType();

		if (type == TypeControl::Control) delete (Control*)v[i];
		if (type == TypeControl::Button) delete (Button*)v[i];
	}
}

template<typename T>
void GUI::Add(T& b)
{
	T* t = new T(b);

	mData->v.push_back(t);
	mData->size++;
}