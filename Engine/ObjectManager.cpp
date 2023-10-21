#include "ObjectManager.h"

#include "Application.h"
#include "Light.h"

int ObjectManager::mSize = 0;
Array<Object*> ObjectManager::mArr;

void ObjectManager::Shotdown()
{
	for (int i = 0; i < mSize; i++)
	{
		delete mArr[i];;
	}
}

void ObjectManager::Clear()
{
	mArr.Clear();
    mSize = 0;
}

int ObjectManager::Size()
{ 
    return mSize;
}