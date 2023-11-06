#include "ObjectManager.h"

#include "Application.h"
#include "Light.h"
#include "Model.h"

Array<Object*> ObjectManager::mArrObject;
Array<Model*> ObjectManager::mArrModel;
Array<DirectionLight*> ObjectManager::mArrDirectionLight;
Array<PointLight*> ObjectManager::mArrPointLight;

int ObjectManager::Add(Model& model)
{
	Model* m = new Model(std::move(model));
	m->mID = mArrObject.Size() + 1;

	mArrObject.Add(m);
	mArrModel.Add(m);

	return m->mID;
}

int ObjectManager::Add(DirectionLight& light)
{
	DirectionLight* l = new DirectionLight(std::move(light));
	l->mID = mArrObject.Size() + 1;

	mArrObject.Add(l);
	mArrDirectionLight.Add(l);

	return l->mID;
}

int ObjectManager::Add(PointLight& light)
{
	PointLight* l = new PointLight(std::move(light));
	l->mID = mArrObject.Size() + 1;

	mArrObject.Add(l);
	mArrPointLight.Add(l);

	return l->mID;
}

template<typename T>
int ObjectManager::Size()
{
	return T();
}

template<>
int ObjectManager::Size<Object>()
{
	return mArrObject.Size();
}

template<>
int ObjectManager::Size<Model>()
{
	return mArrModel.Size();
}

template<>
int ObjectManager::Size<DirectionLight>()
{
	return mArrDirectionLight.Size();
}

template<>
int ObjectManager::Size<PointLight>()
{
	return mArrPointLight.Size();
}

template<typename T>
T* ObjectManager::Get(int)
{
	return 0;
}

template<>
Object* ObjectManager::Get(int id)
{
	int res = BinarySearch(mArrObject, id);

	if (res == -1) return 0;

	return mArrObject[res];
}

template<>
Model* ObjectManager::Get(int id)
{
	return mArrModel[id];
}

template<>
DirectionLight* ObjectManager::Get(int id)
{
	return mArrDirectionLight[id];
}

template<>
PointLight* ObjectManager::Get(int id)
{
	return mArrPointLight[id];
}

void ObjectManager::Shotdown()
{
	int size = mArrObject.Size();
	for (int i = 0; i < size; i++)
	{
		SAFE_DELETE(mArrObject[i]);
	}
}

void ObjectManager::Clear()
{
	mArrModel.Clear();
	mArrDirectionLight.Clear();
	mArrPointLight.Clear();
}