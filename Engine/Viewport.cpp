#include "Viewport.h"

#include "Utility.h"

void Viewport::Update(int x, int y, int width, int height)
{
	FOG_ASSERT(width > 0 && height > 0);

	mViewport.MinDepth = 0.0f;
	mViewport.MaxDepth = 1.0f;
	mViewport.TopLeftX = (float)x;
	mViewport.TopLeftY = (float)y;
	mViewport.Width = (float)width;
	mViewport.Height = (float)height;
}

D3D11_VIEWPORT* Viewport::Get()
{
	return &mViewport;
}