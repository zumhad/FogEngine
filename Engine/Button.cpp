#include "Button.h"

using namespace DirectX;

Button::Button(Button& obj)
{
	x = obj.x;
	y = obj.y;
	width = obj.width;
	height = obj.height;
	action = obj.action;
	color = obj.color;

	VertexBuf vertices[]
	{
		XMFLOAT3{-0.5f, 0.5f, 0.0f},
		XMFLOAT3{0.5f, 0.5f, 0.0f},
		XMFLOAT3{0.5f, -0.5f, 0.0f},
		XMFLOAT3{-0.5f, -0.5f, 0.0f}
	};

	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VertexBuf) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData = {};
	InitData.pSysMem = vertices;
	FOG_TRACE(Direct3D::Device()->CreateBuffer(&bd, &InitData, &mVertexBuffer));

	WORD indices[] =
	{
		0,1,2,
		0,2,3,
	};

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * 6;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = indices;
	FOG_TRACE(Direct3D::Device()->CreateBuffer(&bd, &InitData, &mIndexBuffer));
}

Button::~Button()
{
	SAFE_RELEASE(mIndexBuffer);
	SAFE_RELEASE(mVertexBuffer);
}

void Button::Bind()
{
	UINT stride = sizeof(VertexBuf);
	UINT offset = 0;
	Direct3D::DeviceContext()->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	Direct3D::DeviceContext()->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	Direct3D::DeviceContext()->DrawIndexed(6, 0, 0);
}

XMMATRIX Button::GetWorldMatrix()
{
	int newX = Singlton.editor.width / -2 + x + width / 2;
	int newY = Singlton.editor.height / 2 - y - height / 2;

	//XMMatrixAffineTransformation2D();

	XMMATRIX m = XMMatrixScaling((float)width, (float)height, 0.0f) * XMMatrixTranslation((float)newX, (float)newY, 0.0f);

	return m;
}

void Button::Action()
{
	if (action) action();
}