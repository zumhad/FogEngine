#include "RasterizerState.h"

#include "Direct3D.h"
#include "Utility.h"

RasterizerState::RasterizerState()
{
	mRasterizerState = 0;
}

void RasterizerState::Create()
{
	D3D11_RASTERIZER_DESC desc = {};
	desc.FillMode = D3D11_FILL_SOLID;
	desc.CullMode = D3D11_CULL_BACK;
	desc.FrontCounterClockwise = false;
	desc.DepthClipEnable = true;
	desc.ScissorEnable = true;
	desc.AntialiasedLineEnable = false;
	desc.MultisampleEnable = false;

	FOG_TRACE(Direct3D::Device()->CreateRasterizerState(&desc, &mRasterizerState));
}

ID3D11RasterizerState* RasterizerState::Get()
{
	return mRasterizerState;
}

void RasterizerState::Release()
{
	SAFE_RELEASE(mRasterizerState);
}