#pragma once

#pragma warning(push)
#pragma warning(disable: 4251)

#include "Core.h"

#include "Object.h"
#include "Light.h"
#include "Mesh.h"
#include "LightHelper.h"

#include <vector>

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
    static void Setup();
    static void Shotdown();
    static void Draw();

    static void PrePass();
    static void Pass();

private:
    static int mSize;
    static std::vector<Object*> mArr;
};

template FOG_API void ObjectManager::Add<Object>(Object&);
template FOG_API void ObjectManager::Add<Mesh>(Mesh&);
template FOG_API void ObjectManager::Add<PointLight>(PointLight&);
template FOG_API void ObjectManager::Add<DirectionalLight>(DirectionalLight&);

#pragma warning(pop)