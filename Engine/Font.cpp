#include "Font.h"

#include "Direct3D.h"
#include "Image.h"
#include "Utility.h"

ID3D11ShaderResourceView* Font::mTexture = 0;

void Font::Setup()
{
	int width, height;
	unsigned char* data = Image::Load("Font.png", width, height, ImageType::A);
	
	ID3D11Texture2D* texture;
	{
		D3D11_TEXTURE2D_DESC desc{};
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA subResource{};
		subResource.pSysMem = data;
		subResource.SysMemPitch = width;
		subResource.SysMemSlicePitch = 0;

		FOG_TRACE(Direct3D::Device()->CreateTexture2D(&desc, &subResource, &texture));
	}
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc{};
		desc.Format = DXGI_FORMAT_R8_UNORM;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MostDetailedMip = 0;
		desc.Texture2D.MipLevels = 1;
		FOG_TRACE(Direct3D::Device()->CreateShaderResourceView(texture, &desc, &mTexture));
	}

	SAFE_RELEASE(texture);
	SAFE_DELETE_ARR(data);
}

void Font::Bind()
{
	Direct3D::DeviceContext()->PSSetShaderResources(0, 1, &mTexture);
}

void Font::Shotdown()
{
	SAFE_RELEASE(mTexture);
}