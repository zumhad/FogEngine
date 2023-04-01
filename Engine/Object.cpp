#include "Object.h"

#include "ObjectManager.h"

Object::Object()
{
	mID = 0;
}

int Object::GetID()
{
	return mID;
}