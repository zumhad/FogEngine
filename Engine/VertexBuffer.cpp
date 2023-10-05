#include "VertexBuffer.h"

#include "Utility.h"
#include "Direct3D.h"

#include <DirectXMath.h>

using namespace DirectX;

VertexBuffer::VertexBuffer()
{
	mVertexBuffer = 0;
	mStride = 0;
	mOffset = 0;
}

ID3D11Buffer* const* VertexBuffer::Get()
{
	return &mVertexBuffer;
}

UINT* VertexBuffer::Stride()
{
	return &mStride;
}

UINT* VertexBuffer::Offset()
{
	return &mOffset;
}

void VertexBuffer::Release()
{
	SAFE_RELEASE(mVertexBuffer);
}