#pragma once

#pragma warning(push)
#pragma warning(disable: 4251)

#include "Core.h"

#include "Object.h"
#include "CustomArray.h"

class FOG_API Application;
class FOG_API Direct3D;
class FOG_API Object;
class FOG_API DepthMap;
class FOG_API BufferManager;

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

    template<typename T>
    static T* Get(int id);

    template<typename T>
    static T* Get(Object& obj);

    template<typename T>
    static T* Get(Object* obj);

    static int Size();
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

template<typename T>
T* ObjectManager::Get(int id)
{
    return (T*)mArr[id];
}

template<typename T>
T* ObjectManager::Get(Object& obj)
{
    return (T*)(&obj);
}

template<typename T>
T* ObjectManager::Get(Object* obj)
{
    return (T*)obj;
}

#pragma warning(pop)