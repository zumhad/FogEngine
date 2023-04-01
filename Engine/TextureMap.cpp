#include "TextureMap.h"

#include "Trace.h"
#include "Direct3D.h"
#include "Shader.h"
#include "CustomString.h"
#include "PathHelper.h"

#include <DirectXMath.h>

using namespace DirectX;

ID3D11Buffer* TextureMap::mVertexBuffer = 0;
ID3D11Buffer* TextureMap::mIndexBuffer = 0;

void TextureMap::Setup()
{
	XMFLOAT2 vertices[] =
	{
		XMFLOAT2{ 0.0f, 0.0f },
		XMFLOAT2{ 1.0f, 0.0f },
		XMFLOAT2{ 1.0f, 1.0f },
		XMFLOAT2{ 0.0f, 1.0f }
	};

	UINT indices[] =
	{
		0, 1, 2,
		0, 2, 3
	};

	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(XMFLOAT2) * ARRAYSIZE(vertices);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData{};
	InitData.pSysMem = vertices;
	FOG_TRACE(Direct3D::Device()->CreateBuffer(&bd, &InitData, &mVertexBuffer));

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(UINT) * ARRAYSIZE(indices);
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = indices;
	FOG_TRACE(Direct3D::Device()->CreateBuffer(&bd, &InitData, &mIndexBuffer));
}

void TextureMap::Bind()
{
	static UINT stride = sizeof(XMFLOAT2);
	static UINT offset = 0;

	Direct3D::DeviceContext()->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	Direct3D::DeviceContext()->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	Direct3D::DeviceContext()->DrawIndexed(6, 0, 0);
}

void TextureMap::Shotdown()
{
	SAFE_RELEASE(mVertexBuffer);
	SAFE_RELEASE(mIndexBuffer);
}