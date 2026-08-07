#include "BoundingBox.h"

#include "Utility.h"

void BoundingBox::Create(const Vector3& min, const Vector3& max)
{
    mMin = min;
    mMax = max;

    Array<Vector3> vertex;
    vertex.Add(Vector3(min.x, min.y, min.z));
    vertex.Add(Vector3(max.x, min.y, min.z));
    vertex.Add(Vector3(max.x, min.y, max.z));
    vertex.Add(Vector3(min.x, min.y, max.z));
    vertex.Add(Vector3(min.x, max.y, min.z));
    vertex.Add(Vector3(max.x, max.y, min.z));
    vertex.Add(Vector3(max.x, max.y, max.z));
    vertex.Add(Vector3(min.x, max.y, max.z));

    Array<unsigned int> index;
    index.Add(0); index.Add(1);
    index.Add(1); index.Add(2);
    index.Add(2); index.Add(3);
    index.Add(3); index.Add(0);
    index.Add(0); index.Add(4);
    index.Add(1); index.Add(5);
    index.Add(2); index.Add(6);
    index.Add(3); index.Add(7);
    index.Add(4); index.Add(5);
    index.Add(5); index.Add(6);
    index.Add(6); index.Add(7);
    index.Add(7); index.Add(4);

    mVertexBuffer.Create(vertex);
    mIndexBuffer.Create(index);
}

Vector3 BoundingBox::GetCenter()
{
    return (mMin + mMax) / 2.0f;
}

void BoundingBox::Bind()
{
    Direct3D::DeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

    static unsigned int stride = mVertexBuffer.GetStride();
    static unsigned int offset = mVertexBuffer.GetOffset();
    Direct3D::DeviceContext()->IASetVertexBuffers(0, 1, mVertexBuffer.Get(), &stride, &offset);
    Direct3D::DeviceContext()->IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

    Direct3D::DeviceContext()->DrawIndexed(mIndexBuffer.GetCount(), 0, 0);

    Direct3D::DeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void BoundingBox::Release()
{
    mVertexBuffer.Release();
    mIndexBuffer.Release();
}