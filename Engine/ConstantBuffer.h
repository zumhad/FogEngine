#pragma once

#include "Core.h"

#include "Direct3D.h"
#include "Utility.h"

#include <d3d11.h>

template <typename T>
class FOG_API ConstantBuffer
{
public:
	ConstantBuffer();

	void Create();
	void Bind(T& t);
	void Release();
	ID3D11Buffer* const* Get();

private:
	ID3D11Buffer* mBuffer;
};

template <typename T>
ConstantBuffer<T>::ConstantBuffer()
{
	mBuffer = 0;
}

template <typename T>
void ConstantBuffer<T>::Create()
{
	D3D11_BUFFER_DESC desc{};
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.ByteWidth = sizeof(T);
	FOG_TRACE(Direct3D::Device()->CreateBuffer(&desc, 0, &mBuffer));
}

template <typename T>
void ConstantBuffer<T>::Release()
{
	SAFE_RELEASE(mBuffer);
}

template <typename T>
void ConstantBuffer<T>::Bind(T& t)
{
	static HRESULT hr = 0;

	static D3D11_MAPPED_SUBRESOURCE mappedSubresource{};
	hr = Direct3D::DeviceContext()->Map(mBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);

	if (SUCCEEDED(hr))
	{
		memcpy(mappedSubresource.pData, &t, sizeof(T));
		Direct3D::DeviceContext()->Unmap(mBuffer, 0);
	}
}

template <typename T>
ID3D11Buffer* const* ConstantBuffer<T>::Get()
{
	return &mBuffer;
}