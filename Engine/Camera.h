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
class FOG_API ShadowPass;
class FOG_API LightMap;
class FOG_API Frustum;
class FOG_API Vector3;
class FOG_API Matrix;
class FOG_API SkyboxPass;
class FOG_API OutlinePass;
class FOG_API PrePass;
class FOG_API Picking;
class FOG_API Object;

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
	friend class ShadowPass;
	friend class LightMap;
	friend class SkyboxPass;
	friend class OutlinePass;
	friend class PrePass;
	friend class Picking;
	friend class Object;

public:
	static void SetPosition(const Vector3& position);
	static void SetRotation(const Vector3& rotation);

	static void SetPositionX(float x);
	static void SetPositionY(float y);
	static void SetPositionZ(float z);

	static void SetRotationX(float x);
	static void SetRotationY(float y);
	static void SetRotationZ(float z);

	static void LookAt(const Vector3& pos);
	static void Move(const Vector3& v);
	static void Rotate(const Vector3& v);

	static Vector3 GetPosition();
	static Vector3 GetRotation();
	static Vector3 GetDirection();

	static void SetFOV(float fov);
	static void SetNear(float nearZ);
	static void SetFar(float farZ);

	static float GetFOV();
	static float GetNear();
	static float GetFar();
	static float GetAspectRatio();

private:
	static Vector3& GetOffsetPosition();
	static Vector3& GetRealPosition();
	static Matrix& GetViewMatrix();
	static Matrix& GetProjMatrix();
	static void Setup(const APPCLASS& app);
	static void Update();

private:
	static Vector3 mOffset;
	static Vector3 mPosition;
	static Vector3 mRotation;

	static Matrix mView;
	static Matrix mProj;

	static float mFOV;
	static float mAspectRatio;
	static float mNearZ;
	static float mFarZ;
};