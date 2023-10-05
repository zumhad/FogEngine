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
#include "ColorMap.h"
#include "PipelineState.h"

#include <DirectXCollision.h>

using namespace DirectX;

int ObjectManager::mSize = 0;
Array<Object*> ObjectManager::mArr;

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
	mArr.Clear();
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