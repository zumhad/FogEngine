#pragma once

#include "Core.h"

#include "Vector3.h"
#include "Color.h"
#include "Matrix.h"

enum class TypeObject
{
	DirectionLight,
	PointLight,
	Model
};

class FOG_API ObjectManager;

class FOG_API Object
{
	friend class ObjectManager;

public:
	virtual TypeObject GetType() = 0;
	virtual ~Object() {};

	Object();
	Object(const Object& o);

	void SetPosition(const Vector3& v);
	void SetRotation(const Vector3& v);
	void SetScale(const Vector3& v);

	void SetPositionX(float x);
	void SetPositionY(float y);
	void SetPositionZ(float z);

	void SetRotationX(float x);
	void SetRotationY(float y);
	void SetRotationZ(float z);

	void SetScaleX(float x);
	void SetScaleY(float y);
	void SetScaleZ(float z);

	Vector3 GetPosition();
	Vector3 GetRotation();
	Vector3 GetScale();

	void Move(const Vector3& v);
	void Rotate(const Vector3& v);
	void Scale(const Vector3& v);

	void SetOutline(bool outline);
	bool GetOutline();

	void SetEnable(bool enable);
	bool GetEnable();

	void SetData(int data);
	int GetData();

	int GetID();

protected:
	Matrix GetWorldMatrix();

protected:
	Vector3 mOffset; 
	Vector3 mPosition;
	Vector3 mRotation;
	Vector3 mScale;

	bool mOutline;
	bool mEnable;

	int mData;
	int mID;
};