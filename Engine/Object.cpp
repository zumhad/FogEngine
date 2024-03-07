#include "Object.h"

#include "ObjectManager.h"

Object::Object()
{
	position = Vector3(0.0f, 0.0f, 0.0f);
	rotation = Vector3(0.0f, 0.0f, 0.0f);
	scale = Vector3(1.0f, 1.0f, 1.0f);
	outline = false;
	enable = true;
	data = 0;

	mID = 0;
}

Object::Object(const Object& o) : Object()
{
	position = o.position;
	rotation = o.rotation;
	scale = o.scale;
	outline = o.outline;
	data = o.data;
}

int Object::GetID()
{
	return mID;
}