#include "RasterizerState.h"

#include "Direct3D.h"
#include "Utility.h"

RasterizerState::RasterizerState()
{
	mRasterizerState = 0;
}

void RasterizerState::Create(RasterizerStateType type)
{
	D3D11_RASTERIZER_DESC desc = {};

	switch (type)
	{
		case RasterizerStateType::Default:
		{
			desc.FillMode = D3D11_FILL_SOLID;
			desc.CullMode = D3D11_CULL_BACK;
			desc.FrontCounterClockwise = false;
			desc.DepthClipEnable = true;
			desc.ScissorEnable = true;

			break;
		}

		case RasterizerStateType::Shadow:
		{
			desc.FillMode = D3D11_FILL_SOLID;
			desc.CullMode = D3D11_CULL_FRONT;
			desc.FrontCounterClockwise = false;
			desc.DepthClipEnable = false;
			desc.ScissorEnable = true;
			desc.DepthBias = -100000;
			//desc.SlopeScaledDepthBias = -2.1f;
			//desc.DepthBiasClamp = -0.01f;

			break;
		}

		case RasterizerStateType::Skybox:
		{
			desc.FillMode = D3D11_FILL_SOLID;
			desc.CullMode = D3D11_CULL_FRONT;
			desc.FrontCounterClockwise = false;
			desc.DepthClipEnable = true;
			desc.ScissorEnable = true;

			break;
		}
	}

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