#include "Mesh.h"

#include "Direct3D.h"
#include "Timer.h"
#include "MathHelper.h"
#include "PathHelper.h"
#include "ObjectManager.h"
#include "Application.h"
#include "Utility.h"

#include <WaveFrontReader.h>

using namespace DirectX;

struct Mesh::Data
{
public:
    Data() : indexCount(0), vertexBuffer(0), indexBuffer(0), vertexSize(0) {}
    ~Data();

public:
    Matrix world;
    Matrix worldInvTranspose;

    BoundingBox bb;
    Texture texture;

    int indexCount;
    int vertexSize;
    ID3D11Buffer* vertexBuffer;
    ID3D11Buffer* indexBuffer;
};

Mesh::Data::~Data()
{
    SAFE_RELEASE(indexBuffer);
    SAFE_RELEASE(vertexBuffer);

    texture.Release();
}

Mesh::Mesh()
{
    mData = 0;

    name = L"";
    position = Vector3(0.0f, 0.0f, 0.0f);
    rotation = Vector3(0.0f, 0.0f, 0.0f);
    scale = Vector3(1.0f, 1.0f, 1.0f);
    lighting = true;
    texture = L"white.png";
}

Mesh::Mesh(Mesh& mesh)
{
    mData = new Data;

    position = mesh.position;
    rotation = mesh.rotation;
    scale = mesh.scale;
    lighting = mesh.lighting;
    material = mesh.material;
    name = mesh.name;
    texture = mesh.texture;

    String path = PathHelper::GetAssetsPath();
    path += name;

    // читаем файл
    WaveFrontReader<UINT> wfReader;
    FOG_TRACE(wfReader.Load(path.GetWCHAR()));
    mData->bb = wfReader.bounds;

    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = (UINT)sizeof(wfReader.vertices[0]) * (UINT)wfReader.vertices.size();
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA InitData = {};
    InitData.pSysMem = wfReader.vertices.data();
    FOG_TRACE(Direct3D::Device()->CreateBuffer(&bd, &InitData, &mData->vertexBuffer));

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = (UINT)sizeof(wfReader.indices[0]) * (UINT)wfReader.indices.size();
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    InitData.pSysMem = wfReader.indices.data();
    FOG_TRACE(Direct3D::Device()->CreateBuffer(&bd, &InitData, &mData->indexBuffer));

    mData->indexCount = (int)wfReader.indices.size();
    mData->vertexSize = sizeof(wfReader.vertices[0]);

    mData->texture.Create(texture);
}

void Mesh::Bind()
{
    static UINT stride = mData->vertexSize;
    static UINT offset = 0;
    Direct3D::DeviceContext()->IASetVertexBuffers(0, 1, &mData->vertexBuffer, &stride, &offset);
    Direct3D::DeviceContext()->IASetIndexBuffer(mData->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    mData->texture.Bind();

    Direct3D::DeviceContext()->DrawIndexed(mData->indexCount, 0, 0);
}

BoundingBox Mesh::GetBoundingBox()
{
    BoundingBox bb = mData->bb;

    XMVECTOR orig = XMVectorSet(0, 0, 0, 1);
    XMVECTOR q = XMQuaternionRotationRollPitchYawFromVector(Vector3::ConvertToRadians(rotation));
    XMVECTOR sc = scale;
    XMVECTOR pos = position;

    XMMATRIX m = XMMatrixTransformation(orig, q, sc, orig, q, pos);
    bb.Transform(bb, m);

    return bb;
}

Matrix Mesh::GetWorldMatrix()
{
    XMVECTOR q = XMQuaternionRotationRollPitchYawFromVector(Vector3::ConvertToRadians(rotation));
    Matrix m = XMMatrixAffineTransformation(scale, XMVectorSet(0, 0, 0, 1), q, position);

    return m;
}

Matrix Mesh::GetWorldInvTransposeMatrix()
{
    Matrix m = mData->world;
    //m.m. r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
    m = XMMatrixTranspose(XMMatrixInverse(0, m));
    mData->worldInvTranspose = m;

    return m;
}

Mesh::~Mesh()
{
    SAFE_DELETE(mData);
}


void Mesh::SetPosition(Vector3 v)
{
    position = v;
}

void Mesh::SetRotation(Vector3 v)
{
    rotation = v;
}

void Mesh::SetScale(Vector3 v)
{
    scale = v;
}

Vector3 Mesh::GetPosition()
{
    return position;
}

Vector3 Mesh::GetRotation()
{
    return rotation;
}

Vector3 Mesh::GetScale()
{
    return scale;
}

void Mesh::Move(Vector3 v)
{
    position += v;
}

void Mesh::Rotate(Vector3 v)
{
    rotation += v;
}

void Mesh::Scale(Vector3 v)
{
    v;
}

Vector3 Mesh::GetDirection()
{
    XMVECTOR v = XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f);
    XMVECTOR q = XMQuaternionRotationRollPitchYawFromVector(Vector3::ConvertToRadians(rotation));

    return XMVector3Rotate(v, q);
}