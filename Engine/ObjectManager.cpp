#include "ObjectManager.h"

#include "Application.h"
#include "Light.h"
#include "Mesh.h"
#include "PathHelper.h"
#include "Shader.h"
#include "Direct3D.h"
#include "Camera.h"
#include "FrustumCulling.h"
#include "CustomFile.h"
#include "Cursor.h"
#include "ConstantBuffer.h"
#include "DepthMap.h"
#include "SelectMap.h"
#include "TextureMap.h"
#include "PassMap.h"

#include <DirectXCollision.h>
#include <vector>

using namespace DirectX;
using namespace std;

int ObjectManager::mSize = 0;
vector<Object*> ObjectManager::mArr;

void ObjectManager::Draw()
{
	PrePass();
	Pass();
}

void ObjectManager::PrePass()
{
	{
		std::vector<ID3D11RenderTargetView*> vRTV;
		vRTV.push_back(SelectMap::GetRTV());

		Direct3D::DeviceContext()->OMSetRenderTargets((UINT)vRTV.size(), vRTV.data(), DepthMap::GetDSV());
	}
 
	PassMap::BindSRV();
	DepthMap::BindSRV();
	SelectMap::BindSRV();

	for (int i = 0; i < mSize; i++)
	{
		Object& obj = Get(i);
		TypeObject type = obj.GetType();

		switch (type)
		{
			case TypeObject::Mesh:
			{
				Mesh& mesh = (Mesh&)obj;

				PassMap::Bind(mesh);
				DepthMap::Bind(mesh);
				SelectMap::Bind(mesh);

				mesh.Bind();

				break;
			}
		}
	}

	Direct3D::DeviceContext()->OMSetRenderTargets(0, 0, 0);
}

void ObjectManager::Pass()
{
	Direct3D::DeviceContext()->OMSetRenderTargets(1, Direct3D::RTV(), 0);

	PassMap::BindRTV();
	DepthMap::BindRTV();
	SelectMap::BindRTV();

	TextureMap::Bind();

	PassMap::UnbindRTV();
	DepthMap::UnbindRTV();
	SelectMap::UnbindRTV();

	Direct3D::DeviceContext()->OMSetRenderTargets(0, 0, 0);
}

template<typename T>
void ObjectManager::Add(T& obj)
{
    T* temp = new T(obj);
	temp->mID = mSize;

    mArr.push_back(temp);
    mSize++;
}


void ObjectManager::Setup()
{
	return;
}

void ObjectManager::Shotdown()
{
	for (int i = 0; i < mSize; i++)
	{
		TypeObject type = mArr[i]->GetType();

		if (type == TypeObject::Object) delete (Object*)mArr[i];
		if (type == TypeObject::DirectionalLight) delete (DirectionalLight*)mArr[i];
		if (type == TypeObject::PointLight) delete (PointLight*)mArr[i];
		if (type == TypeObject::Mesh) delete (Mesh*)mArr[i];
	}
}

void ObjectManager::Clear()
{
	mArr.clear();
    mSize = 0;
}


Object& ObjectManager::Get(int id)
{
    if (id + 1 > mSize || id < 0)
        Application::Close();

    return *(mArr[id]);
}

int ObjectManager::Size()
{ 
    return mSize;
}