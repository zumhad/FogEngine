#include "Mesh.h"

#include "Direct3D.h"
#include "Timer.h"
#include "MathHelper.h"
#include "PathHelper.h"
#include "ObjectManager.h"
#include "Application.h"
#include "Utility.h"
#include "Quaternion.h"

#include <WaveFrontReader.h>

using namespace DirectX;

Mesh::Mesh()
{
    mIndexCount = 0;
    mVertexSize = 0;
    mIndexBuffer = 0;
    mVertexBuffer = 0;

    name = L"";
    color = Color(1.0f, 1.0f, 1.0f);
    position = Vector3(0.0f, 0.0f, 0.0f);
    rotation = Vector3(0.0f, 0.0f, 0.0f);
    scale = Vector3(1.0f, 1.0f, 1.0f);
    lighting = true;
    texture = L"white.png";
}

Mesh::Mesh(Mesh& mesh)
{
    position = mesh.position;
    rotation = mesh.rotation;
    scale = mesh.scale;
    lighting = mesh.lighting;
    color = mesh.color;
    name = mesh.name;
    texture = mesh.texture;

    String path = PathHelper::GetAssetsPath();
    path += name;

    WaveFrontReader<UINT> wfReader;
    wfReader.Load(path.GetWCHAR());

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
    mVertexSize = sizeof(wfReader.vertices[0]);

    mTexture.Create(texture);
}

void Mesh::Bind()
{
    static UINT stride = mVertexSize;
    static UINT offset = 0;
    Direct3D::DeviceContext()->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
    Direct3D::DeviceContext()->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

    Direct3D::DeviceContext()->DrawIndexed(mIndexCount, 0, 0);
}

void Mesh::BindTexture()
{
    mTexture.Bind();
}

Matrix Mesh::GetWorldMatrix()
{
    XMVECTOR q = XMQuaternionRotationRollPitchYawFromVector(Vector3::ConvertToRadians(rotation));
    return XMMatrixAffineTransformation(scale, XMVectorSet(0, 0, 0, 1), q, position);;
}

Matrix Mesh::GetWorldInvTransposeMatrix(Matrix& world)
{
    return XMMatrixTranspose(XMMatrixInverse(0, world));
}

Mesh::~Mesh()
{
    SAFE_RELEASE(mIndexBuffer);
    SAFE_RELEASE(mVertexBuffer);

    mTexture.Release();
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

void Mesh::Scale(Vector3)
{
    //scale = v;
}

Vector3 Mesh::GetDirection()
{
    XMVECTOR v = XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f);
    XMVECTOR q = XMQuaternionRotationRollPitchYawFromVector(Vector3::ConvertToRadians(rotation));

    return XMVector3Rotate(v, q);
}