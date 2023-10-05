#include "SamplerState.h"

#include "Utility.h"
#include "Direct3D.h"

SamplerState::SamplerState()
{
	mSamplerState = 0;
}

void SamplerState::Create()
{
	D3D11_SAMPLER_DESC desc{};
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.MaxAnisotropy = 1;
	desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	desc.MipLODBias = 0.0f;
	desc.MinLOD = 0;
	desc.MaxLOD = D3D11_FLOAT32_MAX;

	FOG_TRACE(Direct3D::Device()->CreateSamplerState(&desc, &mSamplerState));
}

void SamplerState::Release()
{
	SAFE_RELEASE(mSamplerState);
}

ID3D11SamplerState* const* SamplerState::Get()
{
	return &mSamplerState;
}