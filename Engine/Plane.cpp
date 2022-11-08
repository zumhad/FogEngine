#include "Plane.h"

#include "Trace.h"
#include "Direct3D.h"

Plane::Plane(Plane& plane)
{
    position = plane.position;
    rotation = plane.rotation;
    scale = plane.scale;
    lighting = plane.lighting;
    material = plane.material;

    mTexture = new Texture;

    Vector3 min = position;
    min.SetX(min.GetX() - scale.GetX());
    min.SetZ(min.GetZ() - scale.GetZ());
    Vector3 max = position;
    max.SetX(max.GetX() + scale.GetX());
    max.SetZ(max.GetZ() + scale.GetZ());
    bb = BoundingBox(min, max);

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

    mTexture->Bind();

    Direct3D::DeviceContext()->DrawIndexed(12, 0, 0);
}

Plane::~Plane()
{
    SAFE_RELEASE(mIndexBuffer);
    SAFE_RELEASE(mVertexBuffer);
    SAFE_DELETE(mTexture);
}

Matrix4& Plane::GetWorldMatrix()
{
    mQRotation = DirectX::XMQuaternionRotationRollPitchYaw(-rotation.GetX(), -rotation.GetY(), rotation.GetZ());
    mWorld = DirectX::XMMatrixAffineTransformation(scale, Quaternion::Identity(), mQRotation, position);

    return mWorld;
}

Matrix4& Plane::GetWorldInvTransposeMatrix()
{
    DirectX::XMMATRIX A = mWorld;
    A.r[3] = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

    DirectX::XMVECTOR det = DirectX::XMMatrixDeterminant(A);
    mWorldInvTranspose = DirectX::XMMatrixTranspose(XMMatrixInverse(&det, A));

    return mWorldInvTranspose;
}