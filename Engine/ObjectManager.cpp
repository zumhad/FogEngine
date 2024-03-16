#include "ObjectManager.h"

#include "Application.h"
#include "Light.h"
#include "Model.h"

Array<Object*> ObjectManager::mArrObject;
Array<Model*> ObjectManager::mArrModel;
Array<DirectionLight*> ObjectManager::mArrDirectionLight;
Array<PointLight*> ObjectManager::mArrPointLight;
int ObjectManager::mSize = 0;

int ObjectManager::Add(Model& model)
{
	Model* m = new Model(std::move(model));
	m->mID = ++mSize;

	mArrObject.Add(m);
	mArrModel.Add(m);

	return m->mID;
}

int ObjectManager::Add(DirectionLight& light)
{
	DirectionLight* l = new DirectionLight(std::move(light));
	l->mID = ++mSize;

	mArrObject.Add(l);
	mArrDirectionLight.Add(l);

	return l->mID;
}

int ObjectManager::Add(PointLight& light)
{
	PointLight* l = new PointLight(std::move(light));
	l->mID = ++mSize;

	if (mArrPointLight.Size() == MAX_POINT_LIGHT)
	{
		l->SetEnable(false);
	}

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
T* ObjectManager::Get(Object* obj)
{
	return 0;
}

template<>
Object* ObjectManager::Get(Object* obj)
{
	return obj;
}

template<>
Model* ObjectManager::Get(Object* obj)
{
	if (obj->GetType() == TypeObject::Model)
		return (Model*)obj;
	else
		return 0;
}

template<>
DirectionLight* ObjectManager::Get(Object* obj)
{
	if (obj->GetType() == TypeObject::DirectionLight)
		return (DirectionLight*)obj;
	else
		return 0;
}

template<>
PointLight* ObjectManager::Get(Object* obj)
{
	if (obj->GetType() == TypeObject::PointLight)
		return (PointLight*)obj;
	else
		return 0;
}

template<typename T>
static T* ObjectManager::GetWithID(int i)
{
	return 0;
}

template<>
Object* ObjectManager::GetWithID(int i)
{
	int res = BinarySearch(mArrObject, i);

	if (res == -1) return 0;

	return mArrObject[res];
}

template<>
Model* ObjectManager::GetWithID(int i)
{
	int res = BinarySearch(mArrModel, i);

	if (res == -1) return 0;

	return mArrModel[res];
}

template<>
DirectionLight* ObjectManager::GetWithID(int i)
{
	int res = BinarySearch(mArrDirectionLight, i);

	if (res == -1) return 0;

	return mArrDirectionLight[res];
}

template<>
PointLight* ObjectManager::GetWithID(int i)
{
	int res = BinarySearch(mArrPointLight, i);

	if (res == -1) return 0;

	return mArrPointLight[res];
}

template<typename T>
T* ObjectManager::GetWithNumber(int i)
{
	return 0;
}

template<>
Object* ObjectManager::GetWithNumber(int i)
{
	if (i < 0 || i > mArrObject.Size() - 1) return 0;

	return mArrObject[i];
}

template<>
Model* ObjectManager::GetWithNumber(int i)
{
	if (i < 0 || i > mArrModel.Size() - 1) return 0;

	return mArrModel[i];
}

template<>
DirectionLight* ObjectManager::GetWithNumber(int i)
{
	if (i < 0 || i > mArrDirectionLight.Size() - 1) return 0;

	return mArrDirectionLight[i];
}

template<>
PointLight* ObjectManager::GetWithNumber(int i)
{
	if (i < 0 || i > mArrPointLight.Size() - 1) return 0;

	return mArrPointLight[i];
}

void ObjectManager::DeleteWithID(int i)
{
	int res = BinarySearch(mArrObject, i);

	if (res == -1) return;

	switch (mArrObject[res]->GetType())
	{
		case TypeObject::Model:
		{
			int n = BinarySearch(mArrModel, i);
			mArrModel.Delete(n);
			break;
		}
		case TypeObject::DirectionLight:
		{
			int n = BinarySearch(mArrDirectionLight, i);
			mArrDirectionLight.Delete(n);
			break;
		}
		case TypeObject::PointLight:
		{
			int n = BinarySearch(mArrPointLight, i);
			mArrPointLight.Delete(n);
			break;
		}
	}

	SAFE_DELETE(mArrObject[res]);
	mArrObject.Delete(res);
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