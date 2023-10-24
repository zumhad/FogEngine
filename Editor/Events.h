#pragma once

#include "Engine.h"

int idFPS = -1;
int idPosX = -1;
int idPosY = -1;
int idPosZ = -1;

void ChangeText(Text& text)
{
	char ch = Input::GetNumber();
	if (ch)
		text.text += String::ToString(ch - '0');

	if (Input::Down(KEY_BACK))
		text.text.Clear();

	if (Input::Down(KEY_PERIOD) || Input::Down(KEY_COMMA))
		text.text += L".";

	if (Input::Down(KEY_MINUS))
		text.text += L"-";
}

void FocusX(Control&)
{
	Text& text = *GUI::Get<Text>(idPosX);
	ChangeText(text);
}

void FocusY(Control&)
{
	Text& text = *GUI::Get<Text>(idPosY);
	ChangeText(text);
}

void FocusZ(Control&)
{
	Text& text = *GUI::Get<Text>(idPosZ);
	ChangeText(text);
}

void FocusOnX(Control& control)
{
	Text& text = *GUI::Get<Text>(idPosX);
	text.color = Color(0.5f, 0.5f, 1.0f);
}

void FocusOnY(Control& control)
{
	Text& text = *GUI::Get<Text>(idPosY);
	text.color = Color(0.5f, 0.5f, 1.0f);
}

void FocusOnZ(Control& control)
{
	Text& text = *GUI::Get<Text>(idPosZ);
	text.color = Color(0.5f, 0.5f, 1.0f);
}

void FocusOffX(Control& control)
{
	Text& text = *GUI::Get<Text>(idPosX);

	text.color = Color(1.0f, 1.0f, 1.0f);
	text.text = String::ToString(atof(text.text));

	Model* model = (Model*)Picking::GetPickObject();
	model->position.x = atof(text.text);
}

void FocusOffY(Control& control)
{
	Text& text = *GUI::Get<Text>(idPosY);

	text.color = Color(1.0f, 1.0f, 1.0f);
	text.text = String::ToString(atof(text.text));

	Model* model = (Model*)Picking::GetPickObject();
	model->position.y = atof(text.text);
}

void FocusOffZ(Control& control)
{
	Text& text = *GUI::Get<Text>(idPosZ);

	text.color = Color(1.0f, 1.0f, 1.0f);
	text.text = String::ToString(atof(text.text));

	Model* model = (Model*)Picking::GetPickObject();
	model->position.z = atof(text.text);
}

void AddBox(Control&)
{
	Model m;
	m.name = L"box.obj";
	ObjectManager::Add(m);
}

void AddPlane(Control&)
{
	Model m;
	m.name = L"plane.obj";
	m.scale = Vector3(100, 1, 100);
	ObjectManager::Add(m);
}

void AddSphere(Control&)
{
	Model m;
	m.name = L"sphere.obj";
	ObjectManager::Add(m);
}

void HoverOn(Control& control)
{
	Static& s = *GUI::Get<Static>(control);
	s.color.r = 1.0f;
}

void HoverOff(Control& control)
{
	Static& s = *GUI::Get<Static>(control);
	s.color.r = 0.05f;
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