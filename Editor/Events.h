#pragma once

#include "Engine.h"

void Focus(Control& control)
{
	/*Text& text = GUI::Get<Text>(control);
	
	if (Input::Down(KEY_BACK))
	{
		if(text.text.Size())
			text.text.Delete(text.text.Size() - 1);
	}
	else
	{
		char ch = Input::GetNumber();
		if (ch)
		{
			text.text += String::ToString(ch - '0');
		}

		if (Input::Down(KEY_PERIOD) || Input::Down(KEY_COMMA))
		{
			text.text += L".";
		}

		if (Input::Down(KEY_MINUS))
		{
			text.text += L"-";
		}
	}*/
}

void FocusOn(Control& control)
{
	//Text& text = GUI::Get<Text>(control);

	//text.color = Color(0.5f, 0.5f, 1.0f);
}

void FocusOffX(Control& control)
{
	/*Text& text = GUI::Get<Text>(control);

	text.color = Color(1.0f, 1.0f, 1.0f);
	text.text = String::ToString(atof(text.text));

	Mesh* mesh = (Mesh*)Picking::GetPickObject();
	mesh->position.x = atof(text.text);*/
}

void FocusOffY(Control& control)
{
	/*Text& text = GUI::Get<Text>(control);

	text.color = Color(0.05f, 0.05f, 0.05f);
	text.text = String::ToString(atof(text.text));

	Mesh* mesh = (Mesh*)Picking::GetPickObject();
	mesh->position.y = atof(text.text);*/
}

void FocusOffZ(Control& control)
{
	/*Text& text = GUI::Get<Text>(control);

	text.color = Color(0.05f, 0.05f, 0.05f);
	text.text = String::ToString(atof(text.text));

	Mesh* mesh = (Mesh*)Picking::GetPickObject();
	mesh->position.z = atof(text.text);*/
}

void AddBox(Control&)
{
	Mesh m;
	m.name = L"box.obj";
	ObjectManager::Add(m);
}

void AddPlane(Control&)
{
	Mesh m;
	m.name = L"plane.obj";
	m.scale = Vector3(100, 1, 100);
	ObjectManager::Add(m);
}

void AddSphere(Control&)
{
	Mesh m;
	m.name = L"sphere.obj";
	ObjectManager::Add(m);
}

void HoverOn(Control& control)
{
	//Static& s = GUI::Get<Static>(control);
	//s.color.r = 1.0f;
}

void HoverOff(Control& control)
{
	//Static& s = GUI::Get<Static>(control);
	//s.color.r = 0.05f;
}

void Close(Control&)
{
	Application::Close();
}

void Restore(Control&)
{
	Application::Restore();
}

void Minimize(Control&)
{
	Application::Minimize();
}