#pragma warning(disable : 4611)
#pragma warning(disable : 6385)
#pragma warning(disable : 6386)

#include "Image.h"

#include "Utility.h"
#include "PathHelper.h"

#include <png.h>

unsigned char* Image::Load(String name, int& width, int& height, ImageType type)
{
	String path = PathHelper::GetAssetsPath();
	path += name;

	png_structp pngStruct{};
	png_infop pngInfo{};
	FILE* file = 0;
	unsigned char* data = 0;

	png_byte colorType;
	png_byte depth;

	file = fopen(path, "rb");
	FOG_ASSERT(file != 0);

	pngStruct = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
	FOG_ASSERT(pngStruct != 0);

	pngInfo = png_create_info_struct(pngStruct);
	FOG_ASSERT(pngInfo != 0);

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
	colorType = png_get_color_type(pngStruct, pngInfo);
	depth = png_get_bit_depth(pngStruct, pngInfo);

	if (depth == 16)
		png_set_strip_16(pngStruct);

	if (colorType == PNG_COLOR_TYPE_PALETTE)
		png_set_palette_to_rgb(pngStruct);

	if (colorType == PNG_COLOR_TYPE_GRAY && depth < 8)
		png_set_expand_gray_1_2_4_to_8(pngStruct);

	if (png_get_valid(pngStruct, pngInfo, PNG_INFO_tRNS))
		png_set_tRNS_to_alpha(pngStruct);

	if (colorType == PNG_COLOR_TYPE_RGB || colorType == PNG_COLOR_TYPE_GRAY || colorType == PNG_COLOR_TYPE_PALETTE)
		png_set_filler(pngStruct, 0xFF, PNG_FILLER_AFTER);

	if (colorType == PNG_COLOR_TYPE_GRAY || colorType == PNG_COLOR_TYPE_GRAY_ALPHA)
		png_set_gray_to_rgb(pngStruct);

	png_read_update_info(pngStruct, pngInfo);

	size_t rowSize = png_get_rowbytes(pngStruct, pngInfo);

	unsigned char** rowPointer = new unsigned char* [height];
	for (int y = 0; y < height; y++)
	{
		rowPointer[y] = new unsigned char[rowSize];
	}

	png_read_image(pngStruct, rowPointer);

	switch (type)
	{
		case ImageType::RGBA:
		{
			data = new unsigned char[rowSize * height];
			for (int i = 0; i < height; i++)
			{
				memcpy(data + (rowSize * i), rowPointer[i], rowSize);
			}

			break;
		}

		case ImageType::A:
		{
			data = new unsigned char[(rowSize / 4) * height];
			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width; j++)
				{
					data[i * width + j] = rowPointer[i][j * 4 + 3];
				}
			}

			break;
		}
	}

	for (int i = 0; i < height; i++)
	{
		SAFE_DELETE_ARR(rowPointer[i]);
	}
	SAFE_DELETE_ARR(rowPointer);

	png_destroy_read_struct(&pngStruct, &pngInfo, 0);
	fclose(file);

	return data;
}