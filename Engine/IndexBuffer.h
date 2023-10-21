#pragma once

#include "Core.h"

#include "CustomArray.h"
#include "Direct3D.h"
#include "Utility.h"

#include <d3d11.h>

class FOG_API TextureMap;

class FOG_API IndexBuffer
{
	friend class TextureMap;

private:
	IndexBuffer();

	template <typename T>
	void Create(Array<T>& arr);

	ID3D11Buffer* Get();
	void Release();
	int Count();

private:
	ID3D11Buffer* mIndexBuffer;
	int mCount;
};

template <typename T>
void IndexBuffer::Create(Array<T>& arr)
{
	mCount = arr.Size();

	D3D11_BUFFER_DESC desc = {};
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(T) * arr.Size();
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = arr.Data();

	FOG_TRACE(Direct3D::Device()->CreateBuffer(&desc, &data, &mIndexBuffer));
}
