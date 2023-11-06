#pragma once

#include "Engine.h"

void UpdateTransform(Button& button, Object* object)
{
	if (object)
	{
		static Button* posX = button.GetChildWithNumber(2);
		static Button* posY = button.GetChildWithNumber(3);
		static Button* posZ = button.GetChildWithNumber(4);
		static Button* rotX = button.GetChildWithNumber(6);
		static Button* rotY = button.GetChildWithNumber(7);
		static Button* rotZ = button.GetChildWithNumber(8);
		static Button* scaleX = button.GetChildWithNumber(10);
		static Button* scaleY = button.GetChildWithNumber(11);
		static Button* scaleZ = button.GetChildWithNumber(12);

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

	static Object* obj = 0;

	if (object)
	{
		if (!obj)
		{
			obj = object;

			Button* b1 = GUI::GetWithID(obj->data);
			Button* b2 = GUI::GetWithID(obj->data + 1);

			b1->rect.color.r = 0.6f;
			b2->rect.color.r = 0.6f;
		}
		else
		{
			if (obj->GetID() != object->GetID())
			{
				Button* b1 = GUI::GetWithID(obj->data);
				Button* b2 = GUI::GetWithID(obj->data + 1);

				b1->rect.color.r = 0.1f;
				b2->rect.color.r = 0.1f;

				obj = object;

				b1 = GUI::GetWithID(obj->data);
				b2 = GUI::GetWithID(obj->data + 1);

				b1->rect.color.r = 0.6f;
				b2->rect.color.r = 0.6f;
			}
		}
	}
	else
	{
		if (obj)
		{
			Button* b1 = GUI::GetWithID(obj->data);
			Button* b2 = GUI::GetWithID(obj->data + 1);

			b1->rect.color.r = 0.1f;
			b2->rect.color.r = 0.1f;

			obj = object;
		}
	}
}