#include "ObjectManager.h"

#include "Application.h"
#include "Light.h"
#include "Mesh.h"
#include "PathHelper.h"
#include "Shader.h"
#include "Trace.h"
#include "Direct3D.h"
#include "Camera.h"
#include "FrustumCulling.h"
#include "CustomFile.h"
#include "Cursor.h"
#include "ConstantBuffer.h"

#include <DirectXCollision.h>

using namespace DirectX;
using namespace std;

ObjectManager::Data* ObjectManager::mData = 0;

struct ObjectManager::Data
{
public:
    Data();
    ~Data();

public:
    int size;
    std::vector<Object*> v;
	Object* selectObject;

    ID3D11VertexShader* vertexShader;
    ID3D11PixelShader* pixelShader;
    ID3D11InputLayout* vertexLayout;

	ConstantBuffer<PerObjectBuffer> perObjectBuffer;
	ConstantBuffer<PerFrameBuffer> perFrameBuffer;
};


ObjectManager::Data::Data() : size(0), vertexShader(0), pixelShader(0), vertexLayout(0), selectObject(0)
{
	String shaderPath;
	PathHelper::GetAssetsPath(shaderPath);
	shaderPath += L"shader.hlsl";

	String includePath;
	PathHelper::GetAssetsPath(includePath);
	includePath += L"light.hlsli";

	String s = L"#define MAX_DIRECTIONAL_LIGHT " + String::ToStr(MAX_DIRECTIONAL_LIGHT);
	File include(includePath, FileOpenMode::Write);
	CHAR* utf8 = s.ToUTF8();
	include.Write(utf8, s.Length());
	delete[] utf8;
	include.Close();

	// Vertex Shader
	ID3DBlob* pVSBlob = 0;
	CompileShaderFromFile(shaderPath, "VS", "vs_5_0", &pVSBlob);
	FOG_TRACE(Direct3D::Device()->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), 0, &vertexShader));

	// Input Layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	FOG_TRACE(Direct3D::Device()->CreateInputLayout(layout, ARRAYSIZE(layout), pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &vertexLayout));
	SAFE_RELEASE(pVSBlob);

	// Pixel Shader
	ID3DBlob* pPSBlob = 0;
	CompileShaderFromFile(shaderPath, "PS", "ps_5_0", &pPSBlob);
	FOG_TRACE(Direct3D::Device()->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), 0, &pixelShader));
	SAFE_RELEASE(pPSBlob);
}

void ObjectManager::Draw()
{
	Direct3D::DeviceContext()->IASetInputLayout(mData->vertexLayout);
	Direct3D::DeviceContext()->VSSetShader(mData->vertexShader, 0, 0);
	Direct3D::DeviceContext()->PSSetShader(mData->pixelShader, 0, 0);

	static PerFrameBuffer frameBuffer{};
	frameBuffer.directionalCount = 0;
	frameBuffer.pointCount = 0;
	frameBuffer.mousePos[0] = Cursor::GetPosition(CURSOR_X);
	frameBuffer.mousePos[1] = Cursor::GetPosition(CURSOR_Y);

	for (int i = 0, d = 0, p = 0; i < mData->size; i++)
	{
		TypeObject type = Get(i).GetType();

		if (type == TypeObject::DirectionalLight)
		{
			DirectionalLight& obj = (DirectionalLight&)Get(i);

			DirectionalLightBuffer light{};
			light.color = obj.color;
			light.direction = obj.direction;

			frameBuffer.directionalLight[d++] = light;
			frameBuffer.directionalCount++;
			frameBuffer.cameraPosW = Camera::GetPosition();
		}

		if (type == TypeObject::PointLight)
		{
			PointLight& obj = (PointLight&)Get(i);

			PointLightBuffer light{};
			light.color = obj.color;
			light.position = obj.position;
			light.range = obj.range;
			light.power = obj.power;

			frameBuffer.pointLight[p++] = light;
			frameBuffer.pointCount++;
			frameBuffer.cameraPosW = Camera::GetPosition();
		}
	}

	mData->perFrameBuffer.Bind(frameBuffer);
	Direct3D::DeviceContext()->PSSetConstantBuffers(0, 1, mData->perFrameBuffer.Get());

	static PerObjectBuffer buffer{};

	for (int i = 0; i < mData->size; i++)
	{
		TypeObject type = Get(i).GetType();

		if (type == TypeObject::Mesh)
		{
			Mesh& obj = (Mesh&)ObjectManager::Get(i);

			BoundingBox bb = obj.GetBoundingBox();
			if (!FrustumCulling::Intersect(bb)) continue;

			XMMATRIX world = XMMatrixTranspose(obj.GetWorldMatrix());
			XMMATRIX worldInvTranspose = XMMatrixTranspose(obj.GetWorldInvTransposeMatrix());
			XMMATRIX worldViewProj = obj.GetWorldMatrix() * Camera::GetViewMatrix() * Camera::GetProjMatrix();

			XMStoreFloat4x4(&buffer.world, world);
			XMStoreFloat4x4(&buffer.worldInvTranspose, worldInvTranspose);
			XMStoreFloat4x4(&buffer.worldViewProj, worldViewProj);
			buffer.material = obj.material;
			buffer.id = obj.id;

			mData->perObjectBuffer.Bind(buffer);
			Direct3D::DeviceContext()->VSSetConstantBuffers(1, 1, mData->perObjectBuffer.Get());
			Direct3D::DeviceContext()->PSSetConstantBuffers(1, 1, mData->perObjectBuffer.Get());

			obj.Bind();
		}
	}
}

void ObjectManager::SetSelectObject(int id)
{
	if (id > 0)
		mData->selectObject = &Get(id - 1);
	else
		mData->selectObject = 0;
}

Object* ObjectManager::GetSelectObject()
{
	Direct3D::Pick();

	return mData->selectObject;
}

ObjectManager::Data::~Data()
{
	perFrameBuffer.Release();
	perObjectBuffer.Release();

	SAFE_RELEASE(vertexShader);
	SAFE_RELEASE(pixelShader);
	SAFE_RELEASE(vertexLayout);

    for (int i = 0; i < size; i++)
    {
        TypeObject type = v[i]->GetType();

        if (type == TypeObject::Object) delete (Object*)v[i];
        if (type == TypeObject::DirectionalLight) delete (DirectionalLight*)v[i];
        if (type == TypeObject::PointLight) delete (PointLight*)v[i];
        if (type == TypeObject::Mesh) delete (Mesh*)v[i];
    }
}

template<typename T>
void ObjectManager::Add(T& obj)
{
    T* temp = new T(obj);

    mData->v.push_back(temp);
    mData->size++;
}


void ObjectManager::Setup()
{
    mData = new Data;
}

void ObjectManager::Shotdown()
{
    SAFE_DELETE(mData);
}

void ObjectManager::Clear()
{
    mData->v.clear();
    mData->size = 0;
}


Object& ObjectManager::Get(int i)
{
    if (i + 1 > mData->size)
        Application::Close();

    return *(mData->v[i]);
}

int ObjectManager::Size()
{ 
    return mData->size;
}