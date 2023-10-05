#include "Texture.h"

#include "Direct3D.h"
#include "PathHelper.h"
#include "Utility.h"

#include <DDSTextureLoader.h>

using namespace DirectX;

void Texture::Release()
{
    SAFE_RELEASE(mTexture);
}

Texture::Texture()
{
    mTexture = 0;
}

void Texture::Create(String name)
{
    String path = PathHelper::GetAssetsPath();
    path += name;

    FOG_TRACE(CreateDDSTextureFromFile(Direct3D::Device(), path.GetWCHAR(), 0, &mTexture));
}

void Texture::Bind()
{
    Direct3D::DeviceContext()->PSSetShaderResources(0, 1, &mTexture);
}