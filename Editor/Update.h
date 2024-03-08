#pragma once

#include "Engine.h"

void UpdateSelectObject(Object* obj)
{
	static Object* currentObj = 0;

	if (obj)
	{
		if (currentObj)
		{
			Button* b1 = GUI::GetWithID(currentObj->data);
			Button* b2 = GUI::GetWithID(currentObj->data + 1);

			if (b1 && b2)
			{
				b1->rect.color.r = 0.1f;
				b2->rect.color.r = 0.1f;
			}

			currentObj->outline = false;

			currentObj = obj;

			currentObj->outline = true;

			b1 = GUI::GetWithID(currentObj->data);
			b2 = GUI::GetWithID(currentObj->data + 1);

			if (b1 && b2)
			{
				b1->rect.color.r = 0.6f;
				b2->rect.color.r = 0.6f;
			}
		}
		else
		{
			currentObj = obj;

			currentObj->outline = true;

			Button* b1 = GUI::GetWithID(currentObj->data);
			Button* b2 = GUI::GetWithID(currentObj->data + 1);

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
			Button* b1 = GUI::GetWithID(currentObj->data);
			Button* b2 = GUI::GetWithID(currentObj->data + 1);

			if (b1 && b2)
			{
				b1->rect.color.r = 0.1f;
				b2->rect.color.r = 0.1f;
			}

			currentObj->outline = false;

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
	if (position != object->position)
	{
		posX->text.text = String::ToString(object->position.x);
		posY->text.text = String::ToString(object->position.y);
		posZ->text.text = String::ToString(object->position.z);

		position = object->position;
	}

	static Vector3 rotation = Vector3(-1.0f, -1.0f, -1.0f);
	if(rotation != object->rotation)
	{
		rotX->text.text = String::ToString(object->rotation.x);
		rotY->text.text = String::ToString(object->rotation.y);
		rotZ->text.text = String::ToString(object->rotation.z);

		rotation = object->rotation;
	}

	static Vector3 scale = Vector3(-1.0f, -1.0f, -1.0f);
	if(scale != object->scale)
	{
		scaleX->text.text = String::ToString(object->scale.x);
		scaleY->text.text = String::ToString(object->scale.y);
		scaleZ->text.text = String::ToString(object->scale.z);

		scale = object->scale;
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
	if (color != model->color)
	{
		colorR->text.text = String::ToString(model->color.r);
		colorG->text.text = String::ToString(model->color.g);
		colorB->text.text = String::ToString(model->color.b);

		color = model->color;
	}

	static float _metallic = -1.0f;
	if (_metallic != model->metallic)
	{
		metallic->text.text = String::ToString(model->metallic);

		_metallic = model->metallic;
	}

	static float _roughness = -1.0f;
	if (_roughness != model->roughness)
	{
		roughness->text.text = String::ToString(model->roughness);

		_roughness = model->roughness;
	}
}