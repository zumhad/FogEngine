#include "TextureMap.h"

#include "Direct3D.h"
#include "Shader.h"
#include "CustomString.h"
#include "PathHelper.h"
#include "Utility.h"
#include "VertexBuffer.h"
#include "CustomArray.h"
#include "Vector2.h"
#include "IndexBuffer.h"

#include <DirectXMath.h>

VertexBuffer TextureMap::mVertexBuffer;
IndexBuffer TextureMap::mIndexBuffer;

void TextureMap::Setup()
{
	Array<Vector2> vertices;
	vertices.Add(Vector2(0.0f, 0.0f));
	vertices.Add(Vector2(1.0f, 0.0f));
	vertices.Add(Vector2(0.0f, 1.0f));
	vertices.Add(Vector2(1.0f, 1.0f));

	Array<UINT> indices;
	indices.Add(0);
	indices.Add(1);
	indices.Add(2);
	indices.Add(1);
	indices.Add(3);
	indices.Add(2);

	mVertexBuffer.Create(vertices);
	mIndexBuffer.Create(indices);
}

void TextureMap::Bind()
{
	Direct3D::DeviceContext()->IASetVertexBuffers(0, 1, mVertexBuffer.Get(), mVertexBuffer.Stride(), mVertexBuffer.Offset());
	Direct3D::DeviceContext()->IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	Direct3D::DeviceContext()->DrawIndexed(mIndexBuffer.Count(), 0, 0);
}

void TextureMap::Shotdown()
{
	mVertexBuffer.Release();
	mIndexBuffer.Release();
}