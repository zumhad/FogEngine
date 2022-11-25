#pragma once

#include "Core.h"
#include "Object.h"
#include "Properties.h"
#include "Light.h"
#include "CustomArray.h"


class FOG_API ObjectManager
{
public:
    static void Shotdown();
    static void Clear();

    template<typename T>
    static void Add(T& obj);

    static int Size() { return mSize; }
    static Object& Get(int i);

private:
    static int mSize;
    static Array<Object*> mArr;
};


template<typename T>
void ObjectManager::Add(T& obj)
{
    T* temp = new T(obj);

    mArr.Add(temp);
    mSize++;
}
