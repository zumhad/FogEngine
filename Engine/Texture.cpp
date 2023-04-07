#include "Texture.h"

#include "Direct3D.h"
#include "PathHelper.h"

#include <DDSTextureLoader.h>

using namespace DirectX;

Texture::~Texture()
{
    SAFE_RELEASE(mTexture);
    SAFE_RELEASE(mSamplerState);
}

Texture::Texture()
{
    String path;
    PathHelper::GetEditorPath(path);
    path += L"img.dds";

    FOG_TRACE(CreateDDSTextureFromFile(Direct3D::Device(), path, 0, &mTexture));

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