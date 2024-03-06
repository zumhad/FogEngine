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

	posX->text.text = String::ToString(object->position.x);
	posY->text.text = String::ToString(object->position.y);
	posZ->text.text = String::ToString(object->position.z);

	rotX->text.text = String::ToString(object->rotation.x);
	rotY->text.text = String::ToString(object->rotation.y);
	rotZ->text.text = String::ToString(object->rotation.z);

	scaleX->text.text = String::ToString(object->scale.x);
	scaleY->text.text = String::ToString(object->scale.y);
	scaleZ->text.text = String::ToString(object->scale.z);
}

void UpdateMaterial(Button* button, Model* model)
{
	static Button* colorR = button->GetChildWithNumber(2);
	static Button* colorG = button->GetChildWithNumber(3);
	static Button* colorB = button->GetChildWithNumber(4);
	static Button* metallic = button->GetChildWithNumber(6);
	static Button* roughness = button->GetChildWithNumber(8);

	colorR->text.text = String::ToString(model->color.r);
	colorG->text.text = String::ToString(model->color.g);
	colorB->text.text = String::ToString(model->color.b);
	metallic->text.text = String::ToString(model->metallic);
	roughness->text.text = String::ToString(model->roughness);
}