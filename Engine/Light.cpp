#include "Light.h"

int DirectionalLight::mCount = 0;
int PointLight::mCount = 0;

DirectionalLight::DirectionalLight()
{
	color = Color(1.0f, 1.0f, 1.0f);
	direction = Vector3(0.0f, -1.0f, 0.0f);
	power = 1.0f;
}

PointLight::PointLight()
{
	color = Color(1.0f, 1.0f, 1.0f);
	position = Vector3(0.0f, 0.0f, 0.0f);
	radius = 1.0f;
	power = 1.0f;
}