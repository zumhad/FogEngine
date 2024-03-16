#pragma once

#pragma warning(push)
#pragma warning(disable : 4251)

#include "Core.h"

#include "CustomArray.h"
#include "Color.h"
#include "CustomString.h"
#include "Matrix.h"
#include "Matrix3.h"
#include "Texture.h"
#include "Object.h"

class FOG_API PipelineState;
class FOG_API ShadowPass;
class FOG_API Mesh;
class FOG_API SkyboxPass;
class FOG_API ObjectManager;
class FOG_API PointLight;
class FOG_API DirectionLight;
class FOG_API OutlinePass;
class FOG_API PrePass;

class FOG_API Model : public Object
{
	friend class PipelineState;
	friend class ShadowPass;
	friend class SkyboxPass;
	friend class ObjectManager;
	friend class PointLight;
	friend class DirectionLight;
	friend class OutlinePass;
	friend class PrePass;

public:
	TypeObject GetType() override { return TypeObject::Model; }

	Model();
	Model(const Model& model);
	Model(Model&& model) noexcept;
	~Model();

	void SetModelPath(const String& modelPath);
	String GetModelPath();
	void SetTexturePath(const String& texturePath);
	String GetTexturePath();
	void SetColor(const Color& color);
	void SetColorR(float r);
	void SetColorG(float g);
	void SetColorB(float b);
	Color GetColor();
	void SetLighting(bool lighting);
	bool GetLighting();
	void SetMetallic(float metallic);
	float GetMetallic();
	void SetRoughness(float roughness);
	float GetRoughness();

private:
	void BindTexture();
	void Draw();

private:
	String mModelPath;
	String mTexturePath;
	Color mColor;
	bool mLighting;
	float mMetallic;
	float mRoughness;

	Array<Mesh*> mArr;
	Texture mTexture;
};

#pragma warning(pop)