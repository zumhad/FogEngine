#include "VertexBuffer.h"

#include "Utility.h"
#include "Direct3D.h"

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

unsigned int VertexBuffer::GetStride()
{
	return mStride;
}

unsigned int VertexBuffer::GetOffset()
{
	return mOffset;
}

void VertexBuffer::Release()
{
	SAFE_RELEASE(mVertexBuffer);
}