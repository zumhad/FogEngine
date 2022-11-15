#include "Cube.h"

#include "Trace.h"
#include "Direct3D.h"


Cube::Cube(Cube& cube)
{
    position = cube.position;
    rotation = cube.rotation;
    scale = cube.scale;
    lighting = cube.lighting;
    material = cube.material;
    
    mTexture = new Texture;

    Vector3 min = position;
    min.SetX(min.GetX() - scale.GetX());
    min.SetY(min.GetY() - scale.GetY());
    min.SetZ(min.GetZ() - scale.GetZ());
    Vector3 max = position;
    max.SetX(max.GetX() + scale.GetX());
    max.SetY(max.GetY() + scale.GetY());
    max.SetZ(max.GetZ() + scale.GetZ());
    bb = BoundingBox(min, max);

    Vertex vertices[] =
    {
        { Vector3(-1.0f, 1.0f, -1.0f), Vector3(0.0f, 1.0f, 0.0f), {0,0}, },
        { Vector3(1.0f, 1.0f, -1.0f), Vector3(0.0f, 1.0f, 0.0f), { 0,1 }, },
        { Vector3(1.0f, 1.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f), { 1,1 },},
        { Vector3(-1.0f, 1.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f), { 1,0 },},

        { Vector3(-1.0f, -1.0f, -1.0f), Vector3(0.0f, -1.0f, 0.0f), {0,0}, },
        { Vector3(1.0f, -1.0f, -1.0f), Vector3(0.0f, -1.0f, 0.0f), { 1,0 }, },
        { Vector3(1.0f, -1.0f, 1.0f), Vector3(0.0f, -1.0f, 0.0f), { 1,1 },},
        { Vector3(-1.0f, -1.0f, 1.0f), Vector3(0.0f, -1.0f, 0.0f), { 0,1 },},

        { Vector3(-1.0f, -1.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f), {0,1}, },
        { Vector3(-1.0f, -1.0f, -1.0f), Vector3(-1.0f, 0.0f, 0.0f), { 1,1 }, },
        { Vector3(-1.0f, 1.0f, -1.0f), Vector3(-1.0f, 0.0f, 0.0f), { 1,0 },},
        { Vector3(-1.0f, 1.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f), { 0,0 },},

        { Vector3(1.0f, -1.0f, 1.0f), Vector3(1.0f, 0.0f, 0.0f), {0,1}, },
        { Vector3(1.0f, -1.0f, -1.0f), Vector3(1.0f, 0.0f, 0.0f), { 1,1 }, },
        { Vector3(1.0f, 1.0f, -1.0f), Vector3(1.0f, 0.0f, 0.0f), { 1,0 },},
        { Vector3(1.0f, 1.0f, 1.0f), Vector3(1.0f, 0.0f, 0.0f), { 0,0 },},

        { Vector3(-1.0f, -1.0f, -1.0f), Vector3(0.0f, 0.0f, -1.0f) , {0,1}, },
        { Vector3(1.0f, -1.0f, -1.0f), Vector3(0.0f, 0.0f, -1.0f), { 1,1 }, },
        { Vector3(1.0f, 1.0f, -1.0f), Vector3(0.0f, 0.0f, -1.0f), { 1,0 },},
        { Vector3(-1.0f, 1.0f, -1.0f), Vector3(0.0f, 0.0f, -1.0f), { 0,0 },},

        { Vector3(-1.0f, -1.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f), {0,1}, },
        { Vector3(1.0f, -1.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f), { 1,1 }, },
        { Vector3(1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f), { 1,0 },},
        { Vector3(-1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f), { 0,0 },},
    };

    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(Vertex) * 24;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA InitData = {};
    InitData.pSysMem = vertices;
    FOG_TRACE(Direct3D::Device()->CreateBuffer(&bd, &InitData, &mVertexBuffer));

    WORD indices[] =
    {
        3,1,0,
        2,1,3,

        6,4,5,
        7,4,6,

        11,9,8,
        10,9,11,

        14,12,13,
        15,12,14,

        19,17,16,
        18,17,19,

        22,20,21,
        23,20,22
    };
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(WORD) * 36;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    InitData.pSysMem = indices;
    FOG_TRACE(Direct3D::Device()->CreateBuffer(&bd, &InitData, &mIndexBuffer));
}

Cube::~Cube()
{
    SAFE_RELEASE(mIndexBuffer);
    SAFE_RELEASE(mVertexBuffer);
    SAFE_DELETE(mTexture);
}


void Cube::Bind()
{
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    Direct3D::DeviceContext()->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
    Direct3D::DeviceContext()->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

    mTexture->Bind();

    Direct3D::DeviceContext()->DrawIndexed(36, 0, 0);
}

Matrix4& Cube::GetWorldMatrix()
{
    mQRotation = DirectX::XMQuaternionRotationRollPitchYaw(-rotation.GetX(), -rotation.GetY(), rotation.GetZ());
    mWorld = DirectX::XMMatrixAffineTransformation(scale, Quaternion::Identity(), mQRotation, position);

    return mWorld;
}

Matrix4& Cube::GetWorldInvTransposeMatrix()
{
    DirectX::XMMATRIX A = mWorld;
    A.r[3] = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

    DirectX::XMVECTOR det = DirectX::XMMatrixDeterminant(A);
    mWorldInvTranspose = DirectX::XMMatrixTranspose(XMMatrixInverse(&det, A));

    return mWorldInvTranspose;
}