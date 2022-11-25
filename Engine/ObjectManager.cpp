#include "ObjectManager.h"

#include "Application.h"
#include "Cube.h"
#include "Plane.h"
#include "Light.h"
#include "Model.h"

Array<Object*> ObjectManager::mArr;
int ObjectManager::mSize = 0;


void ObjectManager::Shotdown()
{
    for (int i = 0; i < mSize; i++)
    {
        TypeObject type = mArr[i]->GetType();

        if (type == TypeObject::Object) delete (Object*)mArr[i];
        if (type == TypeObject::Cube) delete (Cube*)mArr[i];
        if (type == TypeObject::DirectionalLight) delete (DirectionalLight*)mArr[i];
        if (type == TypeObject::Plane) delete (Plane*)mArr[i];
        if (type == TypeObject::PointLight) delete (PointLight*)mArr[i];
        if (type == TypeObject::Model) delete (Model*)mArr[i];
    }
}

void ObjectManager::Clear()
{
    mArr.Clear();
    mSize = 0;
}


Object& ObjectManager::Get(int i)
{
    if (i + 1 > mSize)
        Application::Exit();

    return *(mArr[i]);
}

