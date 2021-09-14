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
        Object* temp = new Object(obj);

        v.push_back(temp);
    }

    static void Add(Cube& cube)
    {
        Cube* temp = new Cube(cube);
        temp->Load();

        v.push_back(temp);
    }

    static int Size() { return v.size(); }

    static Object* Get(int i)
    {

        return v[i];
    }

private:
    static std::vector<Object*> v;
};

