#include "Object.h"

#include "ObjectManager.h"
#include "Quaternion.h"
#include "Camera.h"
#include "MathHelper.h"

Object::Object()
{
	mOffset = Vector3(0.0f, 0.0f, 0.0f);
	mPosition = Vector3(0.0f, 0.0f, 0.0f);
	mRotation = Vector3(0.0f, 0.0f, 0.0f);
	mScale = Vector3(1.0f, 1.0f, 1.0f);

	mOutline = false;
	mEnable = true;

	mData = 0;
	mID = 0;
}

Object::Object(const Object& o) : Object()
{
	mOffset = o.mOffset;
	mPosition = o.mPosition;
	mRotation = o.mRotation;
	mScale = o.mScale;
	mOutline = o.mOutline;
	mEnable = o.mEnable;
	mData = o.mData;
}

void Object::SetPosition(const Vector3& v)
{
	mOffset = Vector3((float)(int)v.x, (float)(int)v.y, (float)(int)v.z);
	mPosition = Vector3(v.x - mOffset.x, v.y - mOffset.y, v.z - mOffset.z);

	mOffset.x = Math::Clamp(mOffset.x, -FLOAT_MAX_NUMBER(2), FLOAT_MAX_NUMBER(2));
	mOffset.y = Math::Clamp(mOffset.y, -FLOAT_MAX_NUMBER(2), FLOAT_MAX_NUMBER(2));
	mOffset.z = Math::Clamp(mOffset.z, -FLOAT_MAX_NUMBER(2), FLOAT_MAX_NUMBER(2));

	if (mOffset.x == -FLOAT_MAX_NUMBER(2) || mOffset.x == FLOAT_MAX_NUMBER(2)) mPosition.x = 0.0f;
	if (mOffset.y == -FLOAT_MAX_NUMBER(2) || mOffset.y == FLOAT_MAX_NUMBER(2)) mPosition.y = 0.0f;
	if (mOffset.z == -FLOAT_MAX_NUMBER(2) || mOffset.z == FLOAT_MAX_NUMBER(2)) mPosition.z = 0.0f;
}

void Object::SetRotation(const Vector3& v)
{
	mRotation = v;
}

void Object::SetScale(const Vector3& v)
{
	mScale = v;
}

void Object::SetPositionX(float x)
{
	mOffset.x = (float)(int)x;
	mPosition.x = x - mOffset.x;

	mOffset.x = Math::Clamp(mOffset.x, -FLOAT_MAX_NUMBER(2), FLOAT_MAX_NUMBER(2));

	if (mOffset.x == -FLOAT_MAX_NUMBER(2) || mOffset.x == FLOAT_MAX_NUMBER(2)) mPosition.x = 0.0f;
}

void Object::SetPositionY(float y)
{
	mOffset.y = (float)(int)y;
	mPosition.y = y - mOffset.y;

	mOffset.y = Math::Clamp(mOffset.y, -FLOAT_MAX_NUMBER(2), FLOAT_MAX_NUMBER(2));

	if (mOffset.y == -FLOAT_MAX_NUMBER(2) || mOffset.y == FLOAT_MAX_NUMBER(2)) mPosition.y = 0.0f;
}

void Object::SetPositionZ(float z)
{
	mOffset.z = (float)(int)z;
	mPosition.z = z - mOffset.z;

	mOffset.z = Math::Clamp(mOffset.z, -FLOAT_MAX_NUMBER(2), FLOAT_MAX_NUMBER(2));

	if (mOffset.z == -FLOAT_MAX_NUMBER(2) || mOffset.z == FLOAT_MAX_NUMBER(2)) mPosition.z = 0.0f;
}

void Object::SetRotationX(float x)
{
	mRotation.x = x;
}

void Object::SetRotationY(float y)
{
	mRotation.y = y;
}

void Object::SetRotationZ(float z)
{
	mRotation.z = z;
}

void Object::SetScaleX(float x)
{
	mScale.x = x;
}

void Object::SetScaleY(float y)
{
	mScale.y = y;
}

void Object::SetScaleZ(float z)
{
	mScale.z = z;
}

Vector3 Object::GetPosition()
{
	return mOffset + mPosition;
}

Vector3 Object::GetRotation()
{
	return mRotation;
}

Vector3 Object::GetScale()
{
	return mScale;
}

void Object::Move(const Vector3& v)
{
	mPosition += v;

	if (mPosition.x >= 1.0f || mPosition.x <= -1.0f)
	{
		mOffset.x += (int)mPosition.x;
		mPosition.x -= (int)mPosition.x;
	}

	if (mPosition.y >= 1.0f || mPosition.y <= -1.0f)
	{
		mOffset.y += (int)mPosition.y;
		mPosition.y -= (int)mPosition.y;
	}

	if (mPosition.z >= 1.0f || mPosition.z <= -1.0f)
	{
		mOffset.z += (int)mPosition.z;
		mPosition.z -= (int)mPosition.z;
	}

	mOffset.x = Math::Clamp(mOffset.x, -FLOAT_MAX_NUMBER(2), FLOAT_MAX_NUMBER(2));
	mOffset.y = Math::Clamp(mOffset.y, -FLOAT_MAX_NUMBER(2), FLOAT_MAX_NUMBER(2));
	mOffset.z = Math::Clamp(mOffset.z, -FLOAT_MAX_NUMBER(2), FLOAT_MAX_NUMBER(2));

	if (mOffset.x == -FLOAT_MAX_NUMBER(2) || mOffset.x == FLOAT_MAX_NUMBER(2)) mPosition.x = 0.0f;
	if (mOffset.y == -FLOAT_MAX_NUMBER(2) || mOffset.y == FLOAT_MAX_NUMBER(2)) mPosition.y = 0.0f;
	if (mOffset.z == -FLOAT_MAX_NUMBER(2) || mOffset.z == FLOAT_MAX_NUMBER(2)) mPosition.z = 0.0f;
}

void Object::Rotate(const Vector3& v)
{
	mRotation += v;
}

void Object::Scale(const Vector3& v)
{
	mScale *= v;
}

void Object::SetOutline(bool outline)
{
	mOutline = outline;
}

bool Object::GetOutline()
{
	return mOutline;
}

void Object::SetEnable(bool enable)
{
	mEnable = enable;
}

bool Object::GetEnable()
{
	return mEnable;
}

void Object::SetData(int data)
{
	mData = data;
}

int Object::GetData()
{
	return mData;
}

int Object::GetID()
{
	return mID;
}

Matrix Object::GetWorldMatrix()
{
	Vector3 position = mOffset + mPosition - Camera::GetOffsetPosition();
	return Matrix::AffineTransformation(position, Vector3::ConvertToRadians(mRotation), mScale);
}