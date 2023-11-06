#pragma once

#include "Engine.h"

int idFPS = -1;
int idSceneTree = -1;
int idProperties = -1;
int idTransform = -1;

int gScrollPos = 0;

void TransformClick(Button& button)
{
	Button* parent = button.GetParent();

	parent->rect.height = 30;

	int size = parent->GetChildCount();
	for (int i = 1; i < size; i++)
	{
		Button* b = parent->GetChildWithNumber(i);
		b->enable = !b->enable;

		if (i == 1)
		{
			if(b->enable) parent->rect.height = 145;
			else parent->rect.height = 30;
		}
	}
}

void SliderPower(Button& button)
{
	float value = Input::GetAxis(MOUSE_X) + atof(button.text.text);
	button.text.text = String::ToString(value);

	Object* obj = Picking::GetPickObject();
	PointLight* light = (PointLight*)obj;

	light->power = value;
}

void SliderPosX(Button& button)
{
	float value = Input::GetAxis(MOUSE_X) + atof(button.text.text);
	button.text.text = String::ToString(value);

	Picking::GetPickObject()->position.x = value;
}

void SliderPosY(Button& button)
{
	float value = Input::GetAxis(MOUSE_X) + atof(button.text.text);
	button.text.text = String::ToString(value);

	Picking::GetPickObject()->position.y = value;
}

void SliderPosZ(Button& button)
{
	float value = Input::GetAxis(MOUSE_X) + atof(button.text.text);
	button.text.text = String::ToString(value);

	Picking::GetPickObject()->position.z = value;
}

void SliderRotX(Button& button)
{
	float value = 10.0f * Input::GetAxis(MOUSE_X) + atof(button.text.text);
	button.text.text = String::ToString(value);

	Picking::GetPickObject()->rotation.x = value;
}

void SliderRotY(Button& button)
{
	float value = 10.0f * Input::GetAxis(MOUSE_X) + atof(button.text.text);
	button.text.text = String::ToString(value);

	Picking::GetPickObject()->rotation.y = value;
}

void SliderRotZ(Button& button)
{
	float value = 10.0f * Input::GetAxis(MOUSE_X) + atof(button.text.text);
	button.text.text = String::ToString(value);

	Picking::GetPickObject()->rotation.z = value;
}

void SliderScaleX(Button& button)
{
	float value = 10.0f * Input::GetAxis(MOUSE_X) + atof(button.text.text);
	button.text.text = String::ToString(value);

	Picking::GetPickObject()->scale.x = value;
}

void SliderScaleY(Button& button)
{
	float value = 10.0f * Input::GetAxis(MOUSE_X) + atof(button.text.text);
	button.text.text = String::ToString(value);

	Picking::GetPickObject()->scale.y = value;
}

void SliderScaleZ(Button& button)
{
	float value = 10.0f * Input::GetAxis(MOUSE_X) + atof(button.text.text);
	button.text.text = String::ToString(value);

	Picking::GetPickObject()->scale.z = value;
}

/*
void ChangeText(Button& button)
{
	char ch = Input::GetNumber();
	if (ch)
		button.text += String::ToString(ch - '0');

	if (Input::Down(KEY_BACK))
		button.text.Clear();

	if (Input::Down(KEY_PERIOD) || Input::Down(KEY_COMMA))
		button.text += L".";

	if (Input::Down(KEY_MINUS))
		button.text += L"-";
}*/

void ScrollTree(Button& button)
{
	int n = Input::GetAxis(MOUSE_SCROLL);

	Button* parent = button.GetParent();
	int size = parent->GetChildCount();

	if (n > 0)
	{
		if (size > 1)
		{
			Button* child = parent->GetChildWithNumber(1);

			if (child->rect.y < 0)
			{
				Button* scrollBar = parent->GetChildWithNumber(0);
				Button* scroll = scrollBar->GetChildWithNumber(0);

				int treeCount = (parent->GetChildCount() - 1) / 2;
				float offset = (1.0f / treeCount) * (scrollBar->rect.height - 6);
				int offsetCount = Math::Round((scroll->rect.y - 3 - offset) / offset);
				scroll->rect.y = Math::Round(offset * offsetCount) + 3;

				for (int i = 1; i < size; i++)
				{
					Button* b = parent->GetChildWithNumber(i);
					b->rect.y += n * b->rect.height;
				}
			}
		}
	}
	else
	{
		if (size > 1)
		{
			Button* child = parent->GetChildWithNumber(size - 1);

			if (child->rect.y + child->rect.height > parent->rect.height)
			{
				Button* scrollBar = parent->GetChildWithNumber(0);
				Button* scroll = scrollBar->GetChildWithNumber(0);

				int treeCount = (parent->GetChildCount() - 1) / 2;
				float offset = (1.0f / treeCount) * (scrollBar->rect.height - 6);
				int offsetCount = Math::Round((scroll->rect.y - 3 + offset) / offset);
				scroll->rect.y = Math::Round(offset * offsetCount) + 3;

				for (int i = 1; i < size; i++)
				{
					Button* b = parent->GetChildWithNumber(i);
					b->rect.y += n * b->rect.height;
				}
			}
		}
	}
}

void UpScroll(Button& button)
{
	gScrollPos = 0;

	button.rect.color = Color(0.35f, 0.35f, 0.35f);
}

void HoverScroll(Button& button)
{
	button.rect.color = Color(0.4f, 0.4f, 0.4f);
}

void PressScroll(Button& button)
{
	button.rect.color = Color(0.45f, 0.45f, 0.45f);

	gScrollPos += Math::Sign(Input::GetAxis(MOUSE_Y));

	Button* parent = button.GetParent()->GetParent();
	int size = parent->GetChildCount();

	if (gScrollPos < 0)
	{
		if (size > 1)
		{
			Button* child = parent->GetChildWithNumber(1);

			if (child->rect.y < 0)
			{
				Button* scrollBar = parent->GetChildWithNumber(0);
				Button* scroll = scrollBar->GetChildWithNumber(0);

				int treeCount = (parent->GetChildCount() - 1) / 2;
				float offset = (1.0f / treeCount) * (scrollBar->rect.height - 6);
				int offsetCount = Math::Round((scroll->rect.y - 3 - offset) / offset);
				int newY = Math::Round(offset * offsetCount) + 3;

				if (scroll->rect.y + gScrollPos <= newY)
				{
					scroll->rect.y = newY;

					for (int i = 1; i < size; i++)
					{
						Button* b = parent->GetChildWithNumber(i);
						b->rect.y += b->rect.height;
					}

					gScrollPos = 0;
				}
			}
			else
			{
				gScrollPos = 0;
			}
		}
	}
	else
	{
		if (size > 1)
		{
			Button* child = parent->GetChildWithNumber(size - 1);

			if (child->rect.y + child->rect.height > parent->rect.height)
			{
				Button* scrollBar = parent->GetChildWithNumber(0);
				Button* scroll = scrollBar->GetChildWithNumber(0);

				int treeCount = (parent->GetChildCount() - 1) / 2;
				float offset = (1.0f / treeCount) * (scrollBar->rect.height - 6);
				int offsetCount = Math::Round((scroll->rect.y - 3 + offset) / offset);
				int newY = Math::Round(offset * offsetCount) + 3;

				if (scroll->rect.y + gScrollPos >= newY)
				{
					scroll->rect.y = newY;

					for (int i = 1; i < size; i++)
					{
						Button* b = parent->GetChildWithNumber(i);
						b->rect.y -= b->rect.height;
					}

					gScrollPos = 0;
				}
			}
			else
			{
				gScrollPos = 0;
			}
		}
	}
}

void ScrollOnScroll(Button& button)
{
	ScrollTree(*button.GetParent());
}

void HoverOn(Button& button)
{
	button.rect.color.r += 0.5;
}

void HoverOff(Button& button)
{
	button.rect.color.r -= 0.5;
}

void HoverOnScroll(Button& button)
{
	button.rect.color = Color(0.4f, 0.4f, 0.4f);
}

void HoverOffScroll(Button& button)
{
	button.rect.color = Color(0.35f, 0.35f, 0.35f);
}

void HoverOnSceneTree(Button& button)
{
	Button* parent = button.GetParent();

	int n = button.GetChildNumber();
	n -= 1 - n % 2;

	Button* child0 = parent->GetChildWithNumber(n);
	Button* child1 = parent->GetChildWithNumber(n + 1);

	Object* obj = Picking::GetPickObject();

	if (obj)
	{
		if (child0->data != Picking::GetPickObject()->GetID())
		{
			child0->rect.color.r += 0.5;
			child1->rect.color.r += 0.5;
		}
	}
	else
	{
		child0->rect.color.r += 0.5;
		child1->rect.color.r += 0.5;
	}
}

void HoverOffSceneTree(Button& button)
{
	Button* parent = button.GetParent();

	int n = button.GetChildNumber();
	n -= 1 - n % 2;

	Button* child0 = parent->GetChildWithNumber(n);
	Button* child1 = parent->GetChildWithNumber(n + 1);

	Object* obj = Picking::GetPickObject();

	if (obj)
	{
		if (child0->data != Picking::GetPickObject()->GetID())
		{
			child0->rect.color.r -= 0.5;
			child1->rect.color.r -= 0.5;
		}
	}
	else
	{
		child0->rect.color.r -= 0.5;
		child1->rect.color.r -= 0.5;
	}
}

void AddOnTree(Button& button)
{
	Picking::SetPickObject(ObjectManager::Get<Object>(button.data));
}

void AddBox(Button&)
{
	Model m;
	m.name = L"box.obj";
	int id = ObjectManager::Add(m);

	static Button* tree = GUI::GetWithID(idSceneTree);
	int size = tree->GetChildCount();

	int offsetY = 0;
	if (size > 1)
	{
		Button* child = tree->GetChildWithNumber(size - 1);
		offsetY = child->rect.y + 30;
	}

	Button b;
	b.text.text = "Box";
	b.rect.x = 5;
	b.rect.y = offsetY;
	b.rect.width = tree->rect.width / 2;
	b.rect.height = 30;
	b.text.alignm.vertical = ALIGNM_CENTER_V;
	b.text.x = 20;
	b.rect.color = Color(0.1f, 0.1f, 0.1f);
	b.event.leftClick = AddOnTree;
	b.event.hoverOn = HoverOnSceneTree;
	b.event.hoverOff = HoverOffSceneTree;
	b.event.scroll = ScrollTree;
	b.data = id;
	int idButton = GUI::AddChild(idSceneTree, b);

	Object* obj = ObjectManager::Get<Object>(id);
 	obj->data = idButton;

	b.rect.x = b.rect.width;
	b.rect.width -= 25;
	b.text.text = "Model";
	GUI::AddChild(idSceneTree, b);

	Button* scrollBar = tree->GetChildWithNumber(0);
	Button* scroll = scrollBar->GetChildWithNumber(0);

	int maxCount = tree->rect.height / 30;
	int treeCount = (tree->GetChildCount() - 1) / 2;

	if (treeCount > 1)
	{
		float offset = (1.0f / (treeCount - 1)) * (scrollBar->rect.height - 6);
		int offsetCount = Math::Round((scroll->rect.y - 3) / offset);

		float newOffset = (1.0f / treeCount) * (scrollBar->rect.height - 6);
		scroll->rect.y = Math::Round(newOffset * offsetCount) + 3;
	}

	scroll->rect.height = Math::Round(Math::Min(float(maxCount) / float(treeCount), 1.0f) * (scrollBar->rect.height - 6));
}

void AddPlane(Button&)
{
	Model m;
	m.name = L"plane.obj";
	m.scale = Vector3(100.0f, 1.0f, 100.0f);
	int id = ObjectManager::Add(m);

	static Button* tree = GUI::GetWithID(idSceneTree);
	int size = tree->GetChildCount();

	int offsetY = 0;
	if (size > 1)
	{
		Button* child = tree->GetChildWithNumber(size - 1);
		offsetY = child->rect.y + 30;
	}

	Button b;
	b.text.text = "Plane";
	b.rect.x = 5;
	b.rect.y = offsetY;
	b.rect.width = tree->rect.width / 2;
	b.rect.height = 30;
	b.text.alignm.vertical = ALIGNM_CENTER_V;
	b.text.x = 20;
	b.rect.color = Color(0.1f, 0.1f, 0.1f);
	b.event.leftClick = AddOnTree;
	b.event.hoverOn = HoverOnSceneTree;
	b.event.hoverOff = HoverOffSceneTree;
	b.event.scroll = ScrollTree;
	b.data = id;
	int idButton = GUI::AddChild(idSceneTree, b);

	Object* obj = ObjectManager::Get<Object>(id);
	obj->data = idButton;

	b.rect.x = b.rect.width;
	b.rect.width -= 25;
	b.text.text = "Model";
	GUI::AddChild(idSceneTree, b);

	Button* scrollBar = tree->GetChildWithNumber(0);
	Button* scroll = scrollBar->GetChildWithNumber(0);

	int maxCount = tree->rect.height / 30;
	int treeCount = (tree->GetChildCount() - 1) / 2;

	if (treeCount > 1)
	{
		float offset = (1.0f / (treeCount - 1)) * (scrollBar->rect.height - 6);
		int offsetCount = Math::Round((scroll->rect.y - 3) / offset);

		float newOffset = (1.0f / treeCount) * (scrollBar->rect.height - 6);
		scroll->rect.y = Math::Round(newOffset * offsetCount) + 3;
	}

	scroll->rect.height = Math::Round(Math::Min(float(maxCount) / float(treeCount), 1.0f) * (scrollBar->rect.height - 6));
}

void AddSphere(Button&)
{
	Model m;
	m.name = L"sphere.obj";
	int id = ObjectManager::Add(m);

	static Button* tree = GUI::GetWithID(idSceneTree);
	int size = tree->GetChildCount();

	int offsetY = 0;
	if (size > 1)
	{
		Button* child = tree->GetChildWithNumber(size - 1);
		offsetY = child->rect.y + 30;
	}

	Button b;
	b.text.text = "Sphere";
	b.rect.x = 5;
	b.rect.y = offsetY;
	b.rect.width = tree->rect.width / 2;
	b.rect.height = 30;
	b.text.alignm.vertical = ALIGNM_CENTER_V;
	b.text.x = 20;
	b.rect.color = Color(0.1f, 0.1f, 0.1f);
	b.event.leftClick = AddOnTree;
	b.event.hoverOn = HoverOnSceneTree;
	b.event.hoverOff = HoverOffSceneTree;
	b.event.scroll = ScrollTree;
	b.data = id;
	int idButton = GUI::AddChild(idSceneTree, b);

	Object* obj = ObjectManager::Get<Object>(id);
	obj->data = idButton;

	b.rect.x = b.rect.width;
	b.rect.width -= 25;
	b.text.text = "Model";
	GUI::AddChild(idSceneTree, b);

	Button* scrollBar = tree->GetChildWithNumber(0);
	Button* scroll = scrollBar->GetChildWithNumber(0);

	int maxCount = tree->rect.height / 30;
	int treeCount = (tree->GetChildCount() - 1) / 2;

	if (treeCount > 1)
	{
		float offset = (1.0f / (treeCount - 1)) * (scrollBar->rect.height - 6);
		int offsetCount = Math::Round((scroll->rect.y - 3) / offset);

		float newOffset = (1.0f / treeCount) * (scrollBar->rect.height - 6);
		scroll->rect.y = Math::Round(newOffset * offsetCount) + 3;
	}

	scroll->rect.height = Math::Round(Math::Min(float(maxCount) / float(treeCount), 1.0f) * (scrollBar->rect.height - 6));
}

void AddRoom()
{
	Model m;
	m.name = L"room.obj";
	m.texture = L"png.png";
	m.position = Vector3(0.0, 0.0, 0.0);
	m.scale = Vector3(1.0, 1.0, 1.0);
	m.rotation = Vector3(0, 0, 0);
	m.color = Color(1.0f, 1.0f, 1.0f);
	int id = ObjectManager::Add(m);

	static Button* tree = GUI::GetWithID(idSceneTree);
	int size = tree->GetChildCount();

	int offsetY = 0;
	if (size > 1)
	{
		Button* child = tree->GetChildWithNumber(size - 1);
		offsetY = child->rect.y + 30;
	}

	Button b;
	b.text.text = "Room";
	b.rect.x = 5;
	b.rect.y = offsetY;
	b.rect.width = tree->rect.width / 2;
	b.rect.height = 30;
	b.text.alignm.vertical = ALIGNM_CENTER_V;
	b.text.x = 20;
	b.rect.color = Color(0.1f, 0.1f, 0.1f);
	b.event.leftClick = AddOnTree;
	b.event.hoverOn = HoverOnSceneTree;
	b.event.hoverOff = HoverOffSceneTree;
	b.event.scroll = ScrollTree;
	b.data = id;
	int idButton = GUI::AddChild(idSceneTree, b);

	Object* obj = ObjectManager::Get<Object>(id);
	obj->data = idButton;

	b.rect.x = b.rect.width;
	b.rect.width -= 25;
	b.text.text = "Model";
	GUI::AddChild(idSceneTree, b);

	Button* scrollBar = tree->GetChildWithNumber(0);
	Button* scroll = scrollBar->GetChildWithNumber(0);

	int maxCount = tree->rect.height / 30;
	int treeCount = (tree->GetChildCount() - 1) / 2;

	if (treeCount > 1)
	{
		float offset = (1.0f / (treeCount - 1)) * (scrollBar->rect.height - 6);
		int offsetCount = Math::Round((scroll->rect.y - 3) / offset);

		float newOffset = (1.0f / treeCount) * (scrollBar->rect.height - 6);
		scroll->rect.y = Math::Round(newOffset * offsetCount) + 3;
	}

	scroll->rect.height = Math::Round(Math::Min(float(maxCount) / float(treeCount), 1.0f) * (scrollBar->rect.height - 6));
}

void Close(Button&)
{
	Application::Close();
}

void Restore(Button&)
{
	Application::Restore();
}

void Minimize(Button&)
{
	Application::Minimize();
}