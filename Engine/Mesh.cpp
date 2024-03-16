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

Mesh::Mesh(const Array<Vertex>& vertex, const Array<unsigned int>& index)
{
    D3D11_BUFFER_DESC desc{};
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.ByteWidth = (UINT)sizeof(vertex[0]) * (UINT)vertex.Size();
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    desc.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA data{};
    data.pSysMem = vertex.Data();
    FOG_TRACE(Direct3D::Device()->CreateBuffer(&desc, &data, &mVertexBuffer));

    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.ByteWidth = (UINT)sizeof(index[0]) * (UINT)index.Size();
    desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    desc.CPUAccessFlags = 0;
    data.pSysMem = index.Data();
    FOG_TRACE(Direct3D::Device()->CreateBuffer(&desc, &data, &mIndexBuffer));

    mIndexCount = index.Size();
    mVertexSize = sizeof(vertex[0]);
}

void Mesh::Draw()
{
    static UINT stride = mVertexSize;
    static UINT offset = 0;
    Direct3D::DeviceContext()->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
    Direct3D::DeviceContext()->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

    Direct3D::DeviceContext()->DrawIndexed(mIndexCount, 0, 0);
}

Mesh::~Mesh()
{
    SAFE_RELEASE(mIndexBuffer);
    SAFE_RELEASE(mVertexBuffer);
}