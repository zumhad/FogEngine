#pragma once

#include "Engine.h"

void UpdateSelectObject(Object* obj)
{
	static Object* currentObj = 0;

	if (obj)
	{
		if (currentObj)
		{
			Button* b1 = GUI::GetWithID(currentObj->GetData());
			Button* b2 = GUI::GetWithID(currentObj->GetData() + 1);

			if (b1 && b2)
			{
				b1->rect.color.r = 0.1f;
				b2->rect.color.r = 0.1f;
			}

			currentObj->SetOutline(false);

			currentObj = obj;

			currentObj->SetOutline(true);

			b1 = GUI::GetWithID(currentObj->GetData());
			b2 = GUI::GetWithID(currentObj->GetData() + 1);

			if (b1 && b2)
			{
				b1->rect.color.r = 0.6f;
				b2->rect.color.r = 0.6f;
			}
		}
		else
		{
			currentObj = obj;

			currentObj->SetOutline(true);

			Button* b1 = GUI::GetWithID(currentObj->GetData());
			Button* b2 = GUI::GetWithID(currentObj->GetData() + 1);

			if (b1 && b2)
			{
				b1->rect.color.r = 0.6f;
				b2->rect.color.r = 0.6f;
			}
		}
	}
	else
	{
		if (currentObj)
		{
			Button* b1 = GUI::GetWithID(currentObj->GetData());
			Button* b2 = GUI::GetWithID(currentObj->GetData() + 1);

			if (b1 && b2)
			{
				b1->rect.color.r = 0.1f;
				b2->rect.color.r = 0.1f;
			}

			currentObj->SetOutline(false);

			currentObj = 0;
		}
	}
}

void UpdateTransform(Button* button, Object* object)
{
	static Button* posX = button->GetChildWithNumber(2);
	static Button* posY = button->GetChildWithNumber(3);
	static Button* posZ = button->GetChildWithNumber(4);
	static Button* rotX = button->GetChildWithNumber(6);
	static Button* rotY = button->GetChildWithNumber(7);
	static Button* rotZ = button->GetChildWithNumber(8);
	static Button* scaleX = button->GetChildWithNumber(10);
	static Button* scaleY = button->GetChildWithNumber(11);
	static Button* scaleZ = button->GetChildWithNumber(12);

	static Vector3 position = Vector3(-1.0f, -1.0f, -1.0f);
	if (position != object->GetPosition())
	{
		posX->text.text = String::ToString(object->GetPosition().x, 2);
		posY->text.text = String::ToString(object->GetPosition().y, 2);
		posZ->text.text = String::ToString(object->GetPosition().z, 2);

		position = object->GetPosition();
	}

	static Vector3 rotation = Vector3(-1.0f, -1.0f, -1.0f);
	if(rotation != object->GetRotation())
	{
		rotX->text.text = String::ToString(object->GetRotation().x, 2);
		rotY->text.text = String::ToString(object->GetRotation().y, 2);
		rotZ->text.text = String::ToString(object->GetRotation().z, 2);

		rotation = object->GetRotation();
	}

	static Vector3 scale = Vector3(-1.0f, -1.0f, -1.0f);
	if(scale != object->GetScale())
	{
		scaleX->text.text = String::ToString(object->GetScale().x, 2);
		scaleY->text.text = String::ToString(object->GetScale().y, 2);
		scaleZ->text.text = String::ToString(object->GetScale().z, 2);

		scale = object->GetScale();
	}
}

void UpdateMaterial(Button* button, Model* model)
{
	static Button* colorR = button->GetChildWithNumber(2);
	static Button* colorG = button->GetChildWithNumber(3);
	static Button* colorB = button->GetChildWithNumber(4);
	static Button* metallic = button->GetChildWithNumber(6);
	static Button* roughness = button->GetChildWithNumber(8);

	static Color color = Color(-1.0f, -1.0f, -1.0f);
	if (color != model->GetColor())
	{
		colorR->text.text = String::ToString(model->GetColor().r, 3);
		colorG->text.text = String::ToString(model->GetColor().g, 3);
		colorB->text.text = String::ToString(model->GetColor().b, 3);

		color = model->GetColor();
	}

	static float _metallic = -1.0f;
	if (_metallic != model->GetMetallic())
	{
		metallic->text.text = String::ToString(model->GetMetallic(), 3);

		_metallic = model->GetMetallic();
	}

	static float _roughness = -1.0f;
	if (_roughness != model->GetRoughness())
	{
		roughness->text.text = String::ToString(model->GetRoughness(), 3);

		_roughness = model->GetRoughness();
	}
}

void UpdateLight(Button* button, DirectionLight* light)
{
	static Button* colorR = button->GetChildWithNumber(2);
	static Button* colorG = button->GetChildWithNumber(3);
	static Button* colorB = button->GetChildWithNumber(4);
	static Button* power = button->GetChildWithNumber(6);

	static Color color = Color(-1.0f, -1.0f, -1.0f);
	if (color != light->GetColor())
	{
		colorR->text.text = String::ToString(light->GetColor().r, 3);
		colorG->text.text = String::ToString(light->GetColor().g, 3);
		colorB->text.text = String::ToString(light->GetColor().b, 3);

		color = light->GetColor();
	}

	static float _power = -1.0f;
	if (_power != light->GetPower())
	{
		power->text.text = String::ToString(light->GetPower(), 2);

		_power = light->GetPower();
	}
}