#include "Model.h"

#include "Direct3D.h"

#include <WaveFrontReader.h>

using namespace DirectX;

Model::Model(Model& model)
{
    position = model.position;
    rotation = model.rotation;
    scale = model.scale;
    lighting = model.lighting;
    material = model.material;
    name = model.name;

    String path;
    PathHelper::GetAssetsPath(path);
    path += name;

    // читаем файл
    WaveFrontReader<WORD> wfReader;
    FOG_TRACE(wfReader.Load(path));

    // создаем bb
    XMVECTOR q = XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&rotation) * (XM_PI / 180.0));
    wfReader.bounds.Transform(wfReader.bounds, 1.0f, q, XMLoadFloat3(&position));
    wfReader.bounds.Extents.x *= scale.x;
    wfReader.bounds.Extents.y *= scale.y;
    wfReader.bounds.Extents.z *= scale.z;
    mBB = wfReader.bounds;

    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = (UINT)sizeof(wfReader.vertices[0]) * (UINT)wfReader.vertices.size();
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA InitData = {};
    InitData.pSysMem = wfReader.vertices.data();
    FOG_TRACE(Direct3D::Device()->CreateBuffer(&bd, &InitData, &mVertexBuffer));

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = (UINT)sizeof(wfReader.indices[0]) * (UINT)wfReader.indices.size();
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    InitData.pSysMem = wfReader.indices.data();
    FOG_TRACE(Direct3D::Device()->CreateBuffer(&bd, &InitData, &mIndexBuffer));

    mIndexCount = (int)wfReader.indices.size();
}

void Model::Bind()
{
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    Direct3D::DeviceContext()->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
    Direct3D::DeviceContext()->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

    //mTexture->Bind();

    Direct3D::DeviceContext()->DrawIndexed(mIndexCount, 0, 0);
}

XMMATRIX Model::GetWorldMatrix()
{
    XMVECTOR q = DirectX::XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&rotation) * (XM_PI / 180.0));
    XMMATRIX m = DirectX::XMMatrixAffineTransformation(XMLoadFloat3(&scale), XMVectorSet(0, 0, 0, 1), q, XMLoadFloat3(&position));
    XMStoreFloat4x4(&mWorld, m);

    return m;
}

XMMATRIX Model::GetWorldInvTransposeMatrix()
{
    XMMATRIX m = XMLoadFloat4x4(&mWorld);
    m.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
    m = XMMatrixTranspose(XMMatrixInverse(0, m));
    XMStoreFloat4x4(&mWorldInvTranspose, m);

    return m;
}

Model::~Model()
{
    SAFE_RELEASE(mIndexBuffer);
    SAFE_RELEASE(mVertexBuffer);
    //SAFE_DELETE(mTexture);
}