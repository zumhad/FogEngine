#pragma once

#include "Core.h"

#include "Trace.h"
#include "Direct3D.h"

#include <d3d11.h>

template <typename T>
class ConstantBuffer
{
public:
	ConstantBuffer();

	void Bind(T data);
	void Release();
	ID3D11Buffer* const* Get();

private:
	ID3D11Buffer* mBuffer;
};

template <typename T>
ConstantBuffer<T>::ConstantBuffer()
{
	D3D11_BUFFER_DESC desc{};
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//desc.ByteWidth = sizeof(T) + (16 - (sizeof(T) % 16 ? (sizeof(T) % 16) : 16));
	desc.ByteWidth = sizeof(T);

	FOG_TRACE(Direct3D::Device()->CreateBuffer(&desc, 0, &mBuffer));
}

template <typename T>
void ConstantBuffer<T>::Bind(T data)
{
	D3D11_MAPPED_SUBRESOURCE mappedSubresource;
	HRESULT hr = Direct3D::DeviceContext()->Map(mBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);

	if (SUCCEEDED(hr))
	{
		memcpy(mappedSubresource.pData, &data, sizeof(T));
		Direct3D::DeviceContext()->Unmap(mBuffer, 0);
	}
}

template <typename T>
void ConstantBuffer<T>::Release()
{
	SAFE_RELEASE(mBuffer);
}

template <typename T>
ID3D11Buffer* const* ConstantBuffer<T>::Get()
{
	return &mBuffer;
}