#pragma once

#include "Core.h"

#include "CustomArray.h"
#include "Direct3D.h"
#include "Utility.h"

#include <d3d11.h>

class FOG_API TextureMap;

class FOG_API VertexBuffer
{
	friend class TextureMap;

private:
	VertexBuffer();

	template <typename T>
	void Create(Array<T>& arr);

	ID3D11Buffer* const* Get();
	UINT* Stride();
	UINT* Offset();
	void Release();

private:
	ID3D11Buffer* mVertexBuffer;

	UINT mStride;
	UINT mOffset;
};

template <typename T>
void VertexBuffer::Create(Array<T>& arr)
{
	mOffset = 0;
	mStride = sizeof(T);

	D3D11_BUFFER_DESC desc = {};
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(T) * arr.Size();
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = arr.Data();

	FOG_TRACE(Direct3D::Device()->CreateBuffer(&desc, &data, &mVertexBuffer));
}

