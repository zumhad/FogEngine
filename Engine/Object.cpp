#include "Object.h"

#include "ObjectManager.h"

Object::Object()
{
	id = ObjectManager::Size() + 1;
}