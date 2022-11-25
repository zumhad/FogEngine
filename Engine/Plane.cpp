#include "Plane.h"

#include "Trace.h"
#include "Direct3D.h"

using namespace DirectX;

Plane::Plane(Plane& plane)
{
    using namespace DirectX::SimpleMath;

    position = plane.position;
    rotation = plane.rotation;
    scale = plane.scale;
    lighting = plane.lighting;
    material = plane.material;

    //mTexture = new Texture;

    XMFLOAT3 min = position;
    min.x = min.x - scale.x;
    min.y = min.y - scale.y;
    min.z = min.z - scale.z;
    XMFLOAT3 max = position;
    max.x = max.x - scale.x;
    max.y = max.y - scale.y;
    max.z = max.z - scale.z;
    mBB = BoundingBox(position, scale);

    Vertex vertices[] =
    {
        { Vector3(-1.0f, 0.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f), { 0, 0 } },
        { Vector3(1.0f, 0.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f), { 0, 1 } },
        { Vector3(1.0f, 0.0f, -1.0f), Vector3(0.0f, 1.0f, 0.0f), { 1, 1 } },
        { Vector3(-1.0f, 0.0f, -1.0f), Vector3(0.0f, 1.0f, 0.0f), { 1, 0 } },
        { Vector3(-1.0f, 0.0f, 1.0f), Vector3(0.0f, -1.0f, 0.0f), { 0, 0 } },
        { Vector3(1.0f, 0.0f, 1.0f), Vector3(0.0f, -1.0f, 0.0f), { 0, 1 } },
        { Vector3(1.0f, 0.0f, -1.0f), Vector3(0.0f, -1.0f, 0.0f), { 1, 1 } },
        { Vector3(-1.0f, 0.0f, -1.0f), Vector3(0.0f, -1.0f, 0.0f), { 1, 0 } }
    };

    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(Vertex) * 8;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA InitData = {};
    InitData.pSysMem = vertices;
    FOG_TRACE(Direct3D::Device()->CreateBuffer(&bd, &InitData, &mVertexBuffer));

    WORD indices[] =
    {
        0,1,2,
        0,2,3,

        4,7,6,
        4,6,5
    };

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(WORD) * 12;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    InitData.pSysMem = indices;
    FOG_TRACE(Direct3D::Device()->CreateBuffer(&bd, &InitData, &mIndexBuffer));
}

void Plane::Bind()
{
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    Direct3D::DeviceContext()->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
    Direct3D::DeviceContext()->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

    //mTexture->Bind();

    Direct3D::DeviceContext()->DrawIndexed(12, 0, 0);
}

Plane::~Plane()
{
    SAFE_RELEASE(mIndexBuffer);
    SAFE_RELEASE(mVertexBuffer);
    SAFE_DELETE(mTexture);
}

DirectX::XMMATRIX Plane::GetWorldMatrix()
{
    XMVECTOR q = DirectX::XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&rotation) * (XM_PI / 180.0));
    XMMATRIX m = DirectX::XMMatrixAffineTransformation(XMLoadFloat3(&scale), XMVectorSet(0, 0, 0, 1), q, XMLoadFloat3(&position));
    XMStoreFloat4x4(&mWorld, m);

    return m;
}

DirectX::XMMATRIX Plane::GetWorldInvTransposeMatrix()
{
    XMMATRIX m = XMLoadFloat4x4(&mWorld);
    m.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
    m = XMMatrixTranspose(XMMatrixInverse(0, m));
    XMStoreFloat4x4(&mWorldInvTranspose, m);

    return m;
}