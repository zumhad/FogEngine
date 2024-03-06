#pragma once

#include "Core.h"

#include <DirectXMath.h>

class FOG_API Application;
class FOG_API ObjectManager;
class FOG_API Cursor;
class FOG_API DepthMap;
class FOG_API SelectMap;
class FOG_API PassMap;
class FOG_API PipelineState;
class FOG_API ShadowMap;
class FOG_API LightMap;
class FOG_API Frustum;
class FOG_API Vector3;
class FOG_API Matrix;
class FOG_API Skybox;
class FOG_API OutlineMap;
class FOG_API PrePass;

struct FOG_API APPCLASS;

class FOG_API Camera
{
	friend class Application;
	friend class ObjectManager;
	friend class Cursor;
	friend class DepthMap;
	friend class SelectMap;
	friend class PassMap;
	friend class PipelineState;
	friend class ShadowMap;
	friend class LightMap;
	friend class Skybox;
	friend class OutlineMap;
	friend class PrePass;

public:
	static void SetPosition(Vector3 position);
	static void SetPosition(float x, float y, float z);
	static void SetPositionX(float x);
	static void SetPositionY(float y);
	static void SetPositionZ(float z);

	static void SetRotation(Vector3 rotation);
	static void SetRotation(float x, float y, float z);
	static void SetRotationX(float x);
	static void SetRotationY(float y);
	static void SetRotationZ(float z);

	static void LookAt(Vector3 pos);
	static void Move(Vector3 v);
	static void Rotate(Vector3 v);
	static float GetRotateX();
	static float GetRotateY();
	static float GetRotateZ();

	static Vector3& GetPosition();
	static Vector3& GetRotation();
	static Vector3 GetDirection();

	static void SetFOV(float fov);
	static void SetNear(float nearZ);
	static void SetFar(float farZ);

	static float GetFOV();
	static float GetNear();
	static float GetFar();
	static float GetAspectRatio();

private:
	static Matrix& GetViewMatrix();
	static Matrix& GetProjMatrix();
	static void Setup(APPCLASS& app);
	static void Update();

private:
	static Vector3 mPosition;
	static Vector3 mRotation;

	static Matrix mView;
	static Matrix mProj;

	static float mFOV;
	static float mAspectRatio;
	static float mNearZ;
	static float mFarZ;
};