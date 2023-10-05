#include "DepthStencilState.h"

#include "Utility.h"
#include "Direct3D.h"

DepthStencilState::DepthStencilState()
{
	mDepthStencilState = 0;
}

void DepthStencilState::Create()
{
	D3D11_DEPTH_STENCIL_DESC desc{};
	desc.DepthEnable = true;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	desc.DepthFunc = D3D11_COMPARISON_GREATER;
	FOG_TRACE(Direct3D::Device()->CreateDepthStencilState(&desc, &mDepthStencilState));
}

void DepthStencilState::Release()
{
	SAFE_RELEASE(mDepthStencilState);
}

ID3D11DepthStencilState* DepthStencilState::Get()
{
	return mDepthStencilState;
}