#pragma once

#include "Core.h"
#include "Object.h"
#include "Properties.h"
#include "Light.h"
#include "Mesh.h"
#include "LightHelper.h"

#include <vector>


class FOG_API ObjectManager
{
public:
    static void Setup();
    static void Shotdown();
    static void Clear();
    static void Draw();

    template<typename T>
    static void Add(T& obj);

    static int Size();
    static Object& Get(int i);

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
    int pointCount; float pad[3];
};


struct PerObjectBuffer
{
    DirectX::XMFLOAT4X4 world;
    DirectX::XMFLOAT4X4 worldInvTranspose;
    DirectX::XMFLOAT4X4 worldViewProj;
    Material material;
};