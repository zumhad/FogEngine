#pragma warning(disable : 6387)
#pragma warning(disable : 4611)

#include "Texture.h"

#include "Direct3D.h"
#include "PathHelper.h"
#include "Utility.h"

#include <png.h>

using namespace DirectX;

void Texture::Release()
{
    SAFE_RELEASE(mTexture);
}

Texture::Texture()
{
    mTexture = 0;
}
#include <vector>
void Texture::Create(String name)
{
    String path = PathHelper::GetAssetsPath();
    path += name;

	png_structp pngStruct{};
	png_infop pngInfo{};
	FILE* file = 0;
	unsigned char* data = 0;

	unsigned int width;
	unsigned int height;
	png_byte type;
	png_byte depth;

	file = fopen(path, "rb");
	FOG_ASSERT(file != 0);

	pngStruct = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
	FOG_ASSERT(pngStruct != 0);

	pngInfo = png_create_info_struct(pngStruct);
	FOG_ASSERT(pngInfo != 0);

	if (setjmp(png_jmpbuf(pngStruct))) 
	{
		FOG_ASSERT(false);
	}

	if (setjmp(png_jmpbuf(pngStruct)))
	{
		png_destroy_read_struct(&pngStruct, &pngInfo, 0);
		fclose(file);
		FOG_ASSERT(false);
	}

	png_init_io(pngStruct, file);
	png_read_info(pngStruct, pngInfo);

	width = png_get_image_width(pngStruct, pngInfo);
	height = png_get_image_height(pngStruct, pngInfo);
	type = png_get_color_type(pngStruct, pngInfo);
	depth = png_get_bit_depth(pngStruct, pngInfo);

	if (depth == 16)
		png_set_strip_16(pngStruct);

	if (type == PNG_COLOR_TYPE_PALETTE)
		png_set_palette_to_rgb(pngStruct);

	if (type == PNG_COLOR_TYPE_GRAY && depth < 8)
		png_set_expand_gray_1_2_4_to_8(pngStruct);

	if (png_get_valid(pngStruct, pngInfo, PNG_INFO_tRNS))
		png_set_tRNS_to_alpha(pngStruct);

	if (type == PNG_COLOR_TYPE_RGB || type == PNG_COLOR_TYPE_GRAY || type == PNG_COLOR_TYPE_PALETTE)
		png_set_filler(pngStruct, 0xFF, PNG_FILLER_AFTER);

	if (type == PNG_COLOR_TYPE_GRAY || type == PNG_COLOR_TYPE_GRAY_ALPHA)
		png_set_gray_to_rgb(pngStruct);

	png_read_update_info(pngStruct, pngInfo);

	size_t rowSize = png_get_rowbytes(pngStruct, pngInfo);

	unsigned char** rowPointer = new unsigned char*[height];
	for (unsigned int y = 0; y < height; y++)
	{
		rowPointer[y] = new unsigned char[rowSize];
	}

	png_read_image(pngStruct, rowPointer);

	data = new unsigned char[rowSize * height];
	for (unsigned int i = 0; i < height; i++)
	{
		memcpy(data + (rowSize * i), rowPointer[i], rowSize);
	}

	png_destroy_read_struct(&pngStruct, &pngInfo, 0);
	fclose(file);

	ID3D11Texture2D* texture;
	{
		D3D11_TEXTURE2D_DESC desc{};
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA subResource{};
		subResource.pSysMem = data;
		subResource.SysMemPitch = width * 4;
		subResource.SysMemSlicePitch = 0;

		FOG_TRACE(Direct3D::Device()->CreateTexture2D(&desc, &subResource, &texture));
	}
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc{};
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MostDetailedMip = 0;
		desc.Texture2D.MipLevels = 1;
		FOG_TRACE(Direct3D::Device()->CreateShaderResourceView(texture, &desc, &mTexture));
	}

	SAFE_RELEASE(texture);
	SAFE_DELETE_ARR(data);
}

void Texture::Bind()
{
    Direct3D::DeviceContext()->PSSetShaderResources(0, 1, &mTexture);
}