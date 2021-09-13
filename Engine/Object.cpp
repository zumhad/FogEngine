#include "Object.h"

using namespace DirectX;

Object::Object()
{
	mWorld = XMMatrixIdentity();
	mScaling = Vector3(1.0f, 1.0f, 1.0f);
	mPosition = Vector3::Zero();
	mQRotation = Quaternion::Zero();
}