#pragma once

#pragma warning(push)
#pragma warning(disable: 4251)

#include "Core.h"

#include "Object.h"
#include "Light.h"
#include "Mesh.h"
#include "LightHelper.h"
#include "CustomArray.h"

class Application;
class Direct3D;
class Object;
class DepthMap;
class BufferManager;

class FOG_API ObjectManager
{
    friend class Application;
    friend class Direct3D;
    friend class Object;
    friend class DepthMap;
    friend class BufferManager;

public:
    template<typename T>
    static void Add(T& obj);

    static int Size();
    static Object& Get(int id);
    static void Clear();

private:
    static void Shotdown();

private:
    static int mSize;
    static Array<Object*> mArr;
};

template<typename T>
void ObjectManager::Add(T& obj)
{
    T* temp = new T(obj);
    temp->mID = mSize;

    mArr.Add(temp);
    mSize++;
}

#pragma warning(pop)