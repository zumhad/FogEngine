#include "IndexBuffer.h"

#include "Direct3D.h"
#include "Utility.h"

IndexBuffer::IndexBuffer()
{
	mIndexBuffer = 0;
	mCount = 0;
}

int IndexBuffer::Count()
{
	return mCount;
}

ID3D11Buffer* IndexBuffer::Get()
{
	return mIndexBuffer;
}

void IndexBuffer::Release()
{
	SAFE_RELEASE(mIndexBuffer);
}