#include "Texture.h"

#include "Trace.h"
#include "Direct3D.h"
#include "DDSTextureLoader.h"
#include "PathHelper.h"

Texture::~Texture()
{
    SAFE_RELEASE(mTexture);
    SAFE_RELEASE(mSamplerState);
}

Texture::Texture()
{
    String path;
    PathHelper::GetEditorPath(path);

    FOG_TRACE(DirectX::CreateDDSTextureFromFile(Direct3D::Device(), path+L"img.dds", nullptr, &mTexture));

    D3D11_SAMPLER_DESC sampDesc = {};
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    Direct3D::Device()->CreateSamplerState(&sampDesc, &mSamplerState);
}

void Texture::Bind()
{
    Direct3D::DeviceContext()->PSSetShaderResources(0, 1, &mTexture);
    Direct3D::DeviceContext()->PSSetSamplers(0, 1, &mSamplerState);
}