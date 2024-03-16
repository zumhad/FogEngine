#include "Light.h"

#include "Quaternion.h"
#include "Model.h"

DirectionLight::DirectionLight() : Object()
{
	mRotation = Vector3(0.0f, -90.0f, 0.0f);

	mColor = Color(1.0f, 1.0f, 1.0f);
	mPower = 1.0f;

	mModel = 0;
}

DirectionLight::DirectionLight(const DirectionLight& light) : Object(light)
{
	mColor = light.mColor;
	mPower = light.mPower;

	mModel = 0;
}

DirectionLight::DirectionLight(DirectionLight&& light) noexcept : DirectionLight(light)
{
	Model model;
	model.SetModelPath(L"sun.obj");
	model.SetLighting(false);

	mModel = new Model(std::move(model));
}

void DirectionLight::Bind()
{
	mModel->Draw();
}

void DirectionLight::BindTexture()
{
	mModel->BindTexture();
}

Model* DirectionLight::GetModel()
{
	mModel->mOffset = mOffset;
	mModel->mPosition = mPosition;
	mModel->mRotation = mRotation;
	mModel->mScale = mScale;
	mModel->mColor = mColor;
	mModel->mID = mID;

	return mModel;
}

DirectionLight::~DirectionLight()
{
	SAFE_DELETE(mModel);
}

Vector3 DirectionLight::GetDirection()
{
	Quaternion q = Quaternion::Euler(Vector3::ConvertToRadians(mRotation));

	return Vector3::Rotate(Vector3(0.0f, 0.0f, 1.0f), q);
}

void DirectionLight::SetColor(const Color& color)
{
	mColor = color;
}

void DirectionLight::SetColorR(float r)
{
	mColor.r = r;
}

void DirectionLight::SetColorG(float g)
{
	mColor.g = g;
}

void DirectionLight::SetColorB(float b)
{
	mColor.b = b;
}

Color DirectionLight::GetColor()
{
	return mColor;
}

void DirectionLight::SetPower(float power)
{
	mPower = power;
}

float DirectionLight::GetPower()
{
	return mPower;
}

PointLight::PointLight() : Object()
{
	mColor = Color(1.0f, 1.0f, 1.0f);
	mRadius = 1.0f;
	mPower = 1.0f;

	mModel = 0;
}

PointLight::PointLight(const PointLight& light) : Object(light)
{
	mColor = light.mColor;
	mRadius = light.mRadius;
	mPower = light.mPower;

	mModel = 0;
}

PointLight::PointLight(PointLight&& light) noexcept : PointLight(light)
{
	Model model;
	model.SetModelPath(L"bulb.obj");
	model.SetLighting(false);

	mModel = new Model(std::move(model));
}

PointLight::~PointLight()
{
	SAFE_DELETE(mModel);
}

void PointLight::Bind()
{
	mModel->Draw();
}

void PointLight::BindTexture()
{
	mModel->BindTexture();
}

Model* PointLight::GetModel()
{
	mModel->mOffset = mOffset;
	mModel->mPosition = mPosition;
	mModel->mRotation = mRotation;
	mModel->mScale = mScale;
	mModel->mColor = mColor;
	mModel->mID = mID;

	return mModel;
}

void PointLight::SetColor(const Color& color)
{
	mColor = color;
}

void PointLight::SetColorR(float r)
{
	mColor.r = r;
}

void PointLight::SetColorG(float g)
{
	mColor.g = g;
}

void PointLight::SetColorB(float b)
{
	mColor.b = b;
}

Color PointLight::GetColor()
{
	return mColor;
}

void PointLight::SetRadius(float radius)
{
	mRadius = radius;
}

float PointLight::GetRadius()
{
	return mRadius;
}

void PointLight::SetPower(float power)
{
	mPower = power;
}

float PointLight::GetPower()
{
	return mPower;
}