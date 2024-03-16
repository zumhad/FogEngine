#include "DepthStencilState.h"

#include "Utility.h"
#include "Direct3D.h"

DepthStencilState::DepthStencilState()
{
	mDepthStencilState = 0;
}

void DepthStencilState::Create(Type type)
{
	D3D11_DEPTH_STENCIL_DESC desc{};

	switch (type)
	{
		case Type::ReadWrite:
		{
			desc.DepthEnable = true;
			desc.DepthFunc = D3D11_COMPARISON_GREATER;
			desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;

			break;
		}

		case Type::Disable:
		{
			desc.DepthEnable = false;
			desc.DepthFunc = D3D11_COMPARISON_NEVER;
			desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

			break;
		}

		case Type::ReadOnly:
		{
			desc.DepthEnable = true;
			desc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;
			desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

			break;
		}
	}

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