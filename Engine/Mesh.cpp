#include "Mesh.h"

#include "Direct3D.h"
#include "Timer.h"
#include "MathHelper.h"
#include "PathHelper.h"
#include "ObjectManager.h"
#include "Application.h"
#include "Utility.h"
#include "Quaternion.h"
#include "Vector2.h"

Mesh::Mesh(const Array<VertexMax>& vertexMax, const Array<VertexMin>& vertexMin, const Array<unsigned int>& index, const Vector3& min, const Vector3& max)
{
    mVertexBufferMin.Create(vertexMin);
    mVertexBufferMax.Create(vertexMax);
    mIndexBuffer.Create(index);
    mBoundingBox.Create(min, max);
}

void Mesh::BindBoundingBox()
{
    mBoundingBox.Bind();
}

void Mesh::Bind(bool normal, bool uv)
{
    if (normal && uv)
    {
        static unsigned int stride = mVertexBufferMax.GetStride();
        static unsigned int offset = mVertexBufferMax.GetOffset();
        Direct3D::DeviceContext()->IASetVertexBuffers(0, 1, mVertexBufferMax.Get(), &stride, &offset);
    }
    
    if(!normal && !uv)
    {
        static unsigned int stride = mVertexBufferMin.GetStride();
        static unsigned int offset = mVertexBufferMin.GetOffset();
        Direct3D::DeviceContext()->IASetVertexBuffers(0, 1, mVertexBufferMin.Get(), &stride, &offset);
    }

    Direct3D::DeviceContext()->IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    Direct3D::DeviceContext()->DrawIndexed(mIndexBuffer.GetCount(), 0, 0);
}

Mesh::~Mesh()
{
    mIndexBuffer.Release();
    mVertexBufferMax.Release();
    mVertexBufferMin.Release();
    mBoundingBox.Release();
}