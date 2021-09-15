#pragma once

#include "Core.h"
#include "MathHelper.h"
using namespace Math;

#include <d3d11_4.h>

#pragma warning(disable: 26812)

class Cube;
class Light;

enum ObjectType
{
	tObject,
	tCube,
	tLight
};


struct Vertex
{
	Vector3 pos;
	Vector3 normal;
};



class FOG_API Object
{
public:
	Object();

	void SetRotation(float x, float y, float z) { mRotation = Vector3(x, y, z); }
	void SetPosition(float x, float y, float z) { mPosition = Vector3(x, y, z); }
	void SetScaling(float x, float y, float z) { mScaling = Vector3(x, y, z); }
	void SetPosition(Vector3 v) { mPosition = v; }
	void SetRotation(Vector3 v) { mRotation = v; }
	void SetScaling(Vector3 v) { mScaling = v; }

	Quaternion GetQRotation() { return mQRotation; };
	Vector3 GetPosition() { return mPosition; }
	Vector3 GetScaling() { return mScaling; }
	Vector3 GetRotation() { return mRotation; }

	void Rotate(float x, float y, float z) { mRotation += Vector3(x, y, z); }
	void RotateX(float angle) { mRotation.SetX(mRotation.GetX() + angle); }
	void RotateY(float angle) { mRotation.SetY(mRotation.GetY() + angle); }
	void RotateZ(float angle) { mRotation.SetZ(mRotation.GetZ() + angle); }
	             
	Matrix4 GetWorldMatrix()
	{ 
		mQRotation = DirectX::XMQuaternionRotationRollPitchYaw(-mRotation.GetX(), -mRotation.GetY(), mRotation.GetZ());
		return DirectX::XMMatrixAffineTransformation(mScaling, Quaternion::Zero(), mQRotation, mPosition);
	}

	ObjectType GetType()
	{
		return mType;
	}


public:
	Vector3 mScaling;
	Vector3 mPosition;
	Vector3 mRotation;

protected:
	ObjectType mType;

private:
	Quaternion mQRotation;
	Matrix4 mWorld;
};
