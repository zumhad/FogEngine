#pragma once

#include "Core.h"
#include "Object.h"
#include "Light.h"
#include "Mesh.h"
#include "LightHelper.h"

#include <vector>

class Application;
class Direct3D;

class FOG_API ObjectManager
{
    friend class Application;
    friend class Direct3D;

public:
    template<typename T>
    static void Add(T& obj);

    static int Size();
    static Object& Get(int i);
    static void Clear();
    static Object* GetSelectObject();

private:
    static void Setup();
    static void Shotdown();
    static void Draw();
    static void SetSelectObject(int id);

private:
    struct Data;
    static Data* mData;
};

template FOG_API void ObjectManager::Add<Object>(Object&);
template FOG_API void ObjectManager::Add<Mesh>(Mesh&);
template FOG_API void ObjectManager::Add<PointLight>(PointLight&);
template FOG_API void ObjectManager::Add<DirectionalLight>(DirectionalLight&);


struct PerFrameBuffer
{
    PointLightBuffer pointLight[MAX_POINT_LIGHT];
    DirectionalLightBuffer directionalLight[MAX_DIRECTIONAL_LIGHT];
    DirectX::XMFLOAT3 cameraPosW;
    int directionalCount;
    int pointCount;
    int mousePos[2]; float pad;
};


struct PerObjectBuffer
{
    DirectX::XMFLOAT4X4 world;
    DirectX::XMFLOAT4X4 worldInvTranspose;
    DirectX::XMFLOAT4X4 worldViewProj;
    Material material;
    int id; float pad[3];
};


struct PickingObject
{
    DirectX::XMFLOAT3 position;
    int id;
    float depth;
};