#include "ScissorRect.h"

#include "Utility.h"

void ScissorRect::Update(int left, int top, int right, int bottom)
{
	FOG_ASSERT(right > left && bottom > top);

	mRect.left = left;
	mRect.top = top;
	mRect.right = right;
	mRect.bottom = bottom;
}

D3D11_RECT* ScissorRect::Get()
{
	return &mRect;
}