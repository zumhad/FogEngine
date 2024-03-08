#include "Light.h"

#include "Quaternion.h"
#include "Model.h"

DirectionLight::DirectionLight() : Object()
{
	color = Color(1.0f, 1.0f, 1.0f);
	rotation = Vector3(0.0f, -90.0f, 0.0f);
	power = 1.0f;

	mModel = 0;
}

DirectionLight::DirectionLight(const DirectionLight& light) : Object(light)
{
	color = light.color;
	power = light.power;

	mModel = 0;
}

DirectionLight::DirectionLight(DirectionLight&& light) noexcept : DirectionLight(light)
{
	Model model;
	model.name = L"sun.obj";
	model.lighting = false;

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
	mModel->position = position;
	mModel->rotation = rotation;
	mModel->scale = scale;
	mModel->color = color;
	mModel->mID = mID;

	return mModel;
}

DirectionLight::~DirectionLight()
{
	SAFE_DELETE(mModel);
}

Vector3 DirectionLight::GetDirection()
{
	Quaternion q = Quaternion::Euler(rotation.x, rotation.y, rotation.z);

	return Vector3::Rotate(Vector3(0, 0, 1), q);
}

PointLight::PointLight() : Object()
{
	color = Color(1.0f, 1.0f, 1.0f);
	radius = 1.0f;
	power = 1.0f;

	mModel = 0;
}

PointLight::PointLight(const PointLight& light) : Object(light)
{
	color = light.color;
	radius = light.radius;
	power = light.power;

	mModel = 0;
}

PointLight::PointLight(PointLight&& light) noexcept : PointLight(light)
{
	Model model;
	model.name = L"bulb.obj";
	model.lighting = false;

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
	mModel->position = position;
	mModel->rotation = rotation;
	mModel->scale = scale;
	mModel->color = color;
	mModel->mID = mID;

	return mModel;
}