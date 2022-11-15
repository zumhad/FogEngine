#pragma once

#include "Core.h"

#include "MathHelper.h"
#include "Direct3D.h"
#include "Trace.h"
#include "Control.h"

struct VertexBuf
{
	DirectX::XMFLOAT3 pos;
};

class FOG_API Button : public Control
{
public:
	TypeControl GetType() override { return TypeControl::Button; }

	Button() {}
	Button(Button& obj)
	{
		x = obj.x;
		y = obj.y;
		width = obj.width;
		height = obj.height;
		action = obj.action;
		color = obj.color;

		VertexBuf vertices[]
		{
			DirectX::XMFLOAT3{-0.5f, 0.5f, 0.0f},
			DirectX::XMFLOAT3{0.5f, 0.5f, 0.0f},
			DirectX::XMFLOAT3{0.5f, -0.5f, 0.0f},
			DirectX::XMFLOAT3{-0.5f, -0.5f, 0.0f}
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

	~Button()
	{
		SAFE_RELEASE(mIndexBuffer);
		SAFE_RELEASE(mVertexBuffer);
	}

	void Bind()
	{
		UINT stride = sizeof(VertexBuf);
		UINT offset = 0;
		Direct3D::DeviceContext()->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
		Direct3D::DeviceContext()->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

		Direct3D::DeviceContext()->DrawIndexed(6, 0, 0);
	}

	Matrix4 GetWorldMatrix() 
	{ 
		int newX = Singlton.editor.width / -2 + x + width / 2;
		int newY = Singlton.editor.height / 2 - y - height / 2;

		return DirectX::XMMatrixScaling((float)width, (float)height, 0.0f) * DirectX::XMMatrixTranslation((float)newX, (float)newY, 0.0f);
	}

	void Action()
	{
		if (action) action();
	}

public:
	Vector4 color;
	int x = 0, y = 0;
	int width = 0, height = 0;
	void (*action)() = 0;

private:
	ID3D11Buffer* mVertexBuffer = 0;
	ID3D11Buffer* mIndexBuffer = 0;
};

