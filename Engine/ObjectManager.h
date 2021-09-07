#pragma once
#include <vector>

#include "Cube.h"
#include "Light.h"

#define GET_OBJECT(x, type) \
((type*)(x->GetRef()))

class FOG_API ObjectManager
{
public:
    static void Add(Object& obj)
    {
        Cube* temp = new Cube;

        v.push_back(temp);
        size++;
    }

    static void Add(Cube& obj)
    {
        Cube* temp = new Cube;
        temp->SetPosition(obj.GetPosition());
        temp->SetRotation(obj.GetRotation());
        temp->SetScaling(obj.GetScaling());

        v.push_back(temp);
        size++;
    }

    static int Length() { return size; }

    static Object* Get(int i)
    {

        return v[i];
    }

private:
    static std::vector<Object*> v;
    static int size;
};

