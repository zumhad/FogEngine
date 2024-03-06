#pragma once

#pragma warning(push)
#pragma warning(disable: 4251)

#include "Core.h"

#include "CustomArray.h"

class FOG_API Application;
class FOG_API Direct3D;
class FOG_API Object;
class FOG_API DepthMap;
class FOG_API BufferManager;
class FOG_API Model;
class FOG_API DirectionLight;
class FOG_API PointLight;

class FOG_API ObjectManager
{
    friend class Application;
    friend class Direct3D;
    friend class Object;
    friend class DepthMap;
    friend class BufferManager;

public:
    template<typename T>
    static T* Get(Object* obj);
    template<>
    Object* Get(Object* obj);
    template<>
    Model* Get(Object* obj);
    template<>
    DirectionLight* Get(Object* obj);
    template<>
    PointLight* Get(Object* obj);

    template<typename T>
    static T* GetWithID(int i);
    template<>
    Object* GetWithID(int i);
    template<>
    Model* GetWithID(int i);
    template<>
    DirectionLight* GetWithID(int i);
    template<>
    PointLight* GetWithID(int i);

    template<typename T>
    static T* GetWithNumber(int i);
    template<>
    Object* GetWithNumber(int i);
    template<>
    Model* GetWithNumber(int i);
    template<>
    DirectionLight* GetWithNumber(int i);
    template<>
    PointLight* GetWithNumber(int i);

    static int Add(Model& model);
    static int Add(DirectionLight& light);
    static int Add(PointLight& light);

    static void DeleteWithID(int i);

    template<typename T>
    static int Size();
    template<>
    int Size<Object>();
    template<>
    int Size<Model>();
    template<>
    int Size<DirectionLight>();
    template<>
    int Size<PointLight>();

    static void Clear();

private:
    static void Shotdown();

    template <typename T>
    static int BinarySearch(Array<T*>& arr, int id);

private:
    static Array<Object*> mArrObject;
    static Array<Model*> mArrModel;
    static Array<DirectionLight*> mArrDirectionLight;
    static Array<PointLight*> mArrPointLight;
    static int mSize;
};

template <typename T>
static int ObjectManager::BinarySearch(Array<T*>& arr, int id)
{
    int size = arr.Size();
    int left = 0;
    int right = size - 1;

    while (left <= right)
    {
        int m = left + (right - left) / 2;

        if (arr[m]->mID == id)
            return m;

        if (arr[m]->mID < id)
            left = m + 1;
        else
            right = m - 1;
    }

    return -1;
}

#pragma warning(pop)