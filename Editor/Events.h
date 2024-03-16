#pragma once

#include "Engine.h"

int idFPS = -1;
int idSceneTree = -1;
int idProperties = -1;
int idTransform = -1;
int idMaterial = -1;
int idLight = -1;

int gScrollPos = 0;

void PropertiesClick(Button& button)
{
	Button* parent = button.GetParent();

	int size = parent->GetChildCount();
	for (int i = 1; i < size; i++)
	{
		Button* b = parent->GetChildWithNumber(i);
		b->enable = !b->enable;

		if (i == 1)
		{
			if(b->enable) parent->rect.height = 140;
			else parent->rect.height = 30;
		}
	}

	static Button* properties = GUI::GetWithID(idProperties);
	int n = parent->GetChildNumber();

	size = properties->GetChildCount();
	for (int i = n + 1; i < size; i++)
	{
		Button* b1 = properties->GetChildWithNumber(i - 1);
		Button* b2 = properties->GetChildWithNumber(i);

		if (!b2->enable) continue;

		int j = i - 2;
		while (!b1->enable)
		{
			b1 = properties->GetChildWithNumber(j--);
		}

		b2->rect.y = b1->rect.y + b1->rect.height + 5;
	}
}

void SliderPower(Button& button)
{
	Object* obj = Picking::GetPickObject();
	DirectionLight* dir = ObjectManager::Get<DirectionLight>(obj);

	float value = 0.01f * Input::GetAxis(MOUSE_X) + dir->GetPower();
	value = Math::Max(0.0f, value);

	button.text.text = String::ToString(value, 2);
	dir->SetPower(value);
}

void SliderPosX(Button& button)
{
	Object* obj = Picking::GetPickObject();

	float value = 0.01f * Input::GetAxis(MOUSE_X) + obj->GetPosition().x;

	button.text.text = String::ToString(value, 2);
	obj->SetPositionX(value);
}

void SliderPosY(Button& button)
{
	Object* obj = Picking::GetPickObject();

	float value = 0.01f * Input::GetAxis(MOUSE_X) + obj->GetPosition().y;

	button.text.text = String::ToString(value, 2);
	obj->SetPositionY(value);
}

void SliderPosZ(Button& button)
{
	Object* obj = Picking::GetPickObject();

	float value = 0.01f * Input::GetAxis(MOUSE_X) + obj->GetPosition().z;

	button.text.text = String::ToString(value, 2);
	obj->SetPositionZ(value);
}

void SliderRotX(Button& button)
{
	Object* obj = Picking::GetPickObject();

	float value = 0.01f * Input::GetAxis(MOUSE_X) + obj->GetRotation().x;

	button.text.text = String::ToString(value, 2);
	obj->SetRotationX(value);
}

void SliderRotY(Button& button)
{
	Object* obj = Picking::GetPickObject();

	float value = 0.01f * Input::GetAxis(MOUSE_X) + obj->GetRotation().y;

	button.text.text = String::ToString(value, 2);
	obj->SetRotationY(value);
}

void SliderRotZ(Button& button)
{
	Object* obj = Picking::GetPickObject();

	float value = 0.01f * Input::GetAxis(MOUSE_X) + obj->GetRotation().z;

	button.text.text = String::ToString(value, 2);
	obj->SetRotationZ(value);
}

void SliderScaleX(Button& button)
{
	Object* obj = Picking::GetPickObject();

	float value = 0.01f * Input::GetAxis(MOUSE_X) + obj->GetScale().x;
	value = Math::Max(value, 0.0f);

	button.text.text = String::ToString(value, 2);
	obj->SetScaleX(value);
}

void SliderScaleY(Button& button)
{
	Object* obj = Picking::GetPickObject();

	float value = 0.01f * Input::GetAxis(MOUSE_X) + obj->GetScale().y;
	value = Math::Max(value, 0.0f);

	button.text.text = String::ToString(value, 2);
	obj->SetScaleY(value);
}

void SliderScaleZ(Button& button)
{
	Object* obj = Picking::GetPickObject();

	float value = 0.01f * Input::GetAxis(MOUSE_X) + obj->GetScale().z;
	value = Math::Max(value, 0.0f);

	button.text.text = String::ToString(value, 2);
	obj->SetScaleZ(value);
}

void SliderColorR(Button& button)
{
	Object* obj = Picking::GetPickObject();
	Model* model = ObjectManager::Get<Model>(obj);

	float value = 0.001f * Input::GetAxis(MOUSE_X) + model->GetColor().r;
	value = Math::Clamp(value, 0.0f, 1.0f);

	button.text.text = String::ToString(value, 3);
	model->SetColorR(value);
}

void SliderColorG(Button& button)
{
	Object* obj = Picking::GetPickObject();
	Model* model = ObjectManager::Get<Model>(obj);

	float value = 0.001f * Input::GetAxis(MOUSE_X) + model->GetColor().g;
	value = Math::Clamp(value, 0.0f, 1.0f);

	button.text.text = String::ToString(value, 3);
	model->SetColorG(value);
}

void SliderColorB(Button& button)
{
	Object* obj = Picking::GetPickObject();
	Model* model = ObjectManager::Get<Model>(obj);

	float value = 0.001f * Input::GetAxis(MOUSE_X) + model->GetColor().b;
	value = Math::Clamp(value, 0.0f, 1.0f);

	button.text.text = String::ToString(value, 3);
	model->SetColorB(value);
}

void SliderLightColorR(Button& button)
{
	Object* obj = Picking::GetPickObject();
	DirectionLight* model = ObjectManager::Get<DirectionLight>(obj);

	float value = 0.001f * Input::GetAxis(MOUSE_X) + model->GetColor().r;
	value = Math::Clamp(value, 0.0f, 1.0f);

	button.text.text = String::ToString(value, 3);
	model->SetColorR(value);
}

void SliderLightColorG(Button& button)
{
	Object* obj = Picking::GetPickObject();
	DirectionLight* model = ObjectManager::Get<DirectionLight>(obj);

	float value = 0.001f * Input::GetAxis(MOUSE_X) + model->GetColor().g;
	value = Math::Clamp(value, 0.0f, 1.0f);

	button.text.text = String::ToString(value, 3);
	model->SetColorG(value);
}

void SliderLightColorB(Button& button)
{
	Object* obj = Picking::GetPickObject();
	DirectionLight* model = ObjectManager::Get<DirectionLight>(obj);

	float value = 0.001f * Input::GetAxis(MOUSE_X) + model->GetColor().b;
	value = Math::Clamp(value, 0.0f, 1.0f);

	button.text.text = String::ToString(value, 3);
	model->SetColorB(value);
}

void SliderMetallic(Button& button)
{
	Object* obj = Picking::GetPickObject();
	Model* model = ObjectManager::Get<Model>(obj);

	float value = 0.001f * Input::GetAxis(MOUSE_X) + model->GetMetallic();
	value = Math::Clamp(value, 0.0f, 1.0f);

	button.text.text = String::ToString(value, 3);
	model->SetMetallic(value);
}

void SliderRoughness(Button& button)
{
	Object* obj = Picking::GetPickObject();
	Model* model = ObjectManager::Get<Model>(obj);

	float value = 0.001f * Input::GetAxis(MOUSE_X) + model->GetRoughness();
	value = Math::Clamp(value, 0.0f, 1.0f);

	button.text.text = String::ToString(value, 3);
	model->SetRoughness(value);
}

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

	gScrollPos += Input::GetAxis(MOUSE_Y);

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
		if (obj->GetID() != child0->data)
		{
			child0->rect.color.r = 0.6f;
			child1->rect.color.r = 0.6f;
		}
	}
	else
	{
		child0->rect.color.r = 0.6f;
		child1->rect.color.r = 0.6f;
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
		if (obj->GetID() != child0->data)
		{
			child0->rect.color.r = 0.1f;
			child1->rect.color.r = 0.1f;
		}
	}
	else
	{
		child0->rect.color.r = 0.1f;
		child1->rect.color.r = 0.1f;
	}
}

void AddOnTree(Button& button)
{
	Object* obj = ObjectManager::GetWithID<Object>(button.data);
	Picking::SetPickObject(obj);
}

void AddBox(Button&)
{
	Model m;
	m.SetModelPath(L"box.obj");
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
	b.text.text = L"Box";
	b.rect.x = 5;
	b.rect.y = offsetY;
	b.rect.width = tree->rect.width / 2;
	b.rect.height = 30;
	b.text.alignm.vertical = ALIGNM_CENTER_V;
	b.text.x = 20;
	b.rect.color = Color(0.1f, 0.1f, 0.1f);
	b.event.leftDown = AddOnTree;
	b.event.hoverOn = HoverOnSceneTree;
	b.event.hoverOff = HoverOffSceneTree;
	b.event.scroll = ScrollTree;
	b.data = id;
	int idButton = GUI::AddChild(idSceneTree, b);

	Object* obj = ObjectManager::GetWithNumber<Object>(ObjectManager::Size<Object>() - 1);
 	obj->SetData(idButton);

	b.rect.x = b.rect.width;
	b.rect.width -= 25;
	b.text.text = L"Model";
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

	Picking::SetPickObject(obj);
}

void AddPlane(Button&)
{
	Model m;
	m.SetModelPath(L"plane.obj");
	m.SetScale(Vector3(100.0f, 1.0f, 100.0f));
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
	b.text.text = L"Plane";
	b.rect.x = 5;
	b.rect.y = offsetY;
	b.rect.width = tree->rect.width / 2;
	b.rect.height = 30;
	b.text.alignm.vertical = ALIGNM_CENTER_V;
	b.text.x = 20;
	b.rect.color = Color(0.1f, 0.1f, 0.1f);
	b.event.leftDown = AddOnTree;
	b.event.hoverOn = HoverOnSceneTree;
	b.event.hoverOff = HoverOffSceneTree;
	b.event.scroll = ScrollTree;
	b.data = id;
	int idButton = GUI::AddChild(idSceneTree, b);

	Object* obj = ObjectManager::GetWithNumber<Object>(ObjectManager::Size<Object>() - 1);
	obj->SetData(idButton);

	b.rect.x = b.rect.width;
	b.rect.width -= 25;
	b.text.text = L"Model";
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

	Picking::SetPickObject(obj);
}

void AddSphere(Button&)
{
	Model m;
	m.SetModelPath(L"sphere.obj");
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
	b.text.text = L"Sphere";
	b.rect.x = 5;
	b.rect.y = offsetY;
	b.rect.width = tree->rect.width / 2;
	b.rect.height = 30;
	b.text.alignm.vertical = ALIGNM_CENTER_V;
	b.text.x = 20;
	b.rect.color = Color(0.1f, 0.1f, 0.1f);
	b.event.leftDown = AddOnTree;
	b.event.hoverOn = HoverOnSceneTree;
	b.event.hoverOff = HoverOffSceneTree;
	b.event.scroll = ScrollTree;
	b.data = id;
	int idButton = GUI::AddChild(idSceneTree, b);

	Object* obj = ObjectManager::GetWithNumber<Object>(ObjectManager::Size<Object>() - 1);
	obj->SetData(idButton);

	b.rect.x = b.rect.width;
	b.rect.width -= 25;
	b.text.text = L"Model";
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

	Picking::SetPickObject(obj);
}

void AddRoom()
{
	Model m;
	m.SetModelPath(L"abandonhouse.obj");
	m.SetTexturePath(L"AbHouse_Base_Color.png");
	m.SetPosition(Vector3(0.0f, 0.0f, 0.0f));
	m.SetScale(Vector3(0.01f, 0.01f, 0.01f));
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
	b.text.text = L"Room";
	b.rect.x = 5;
	b.rect.y = offsetY;
	b.rect.width = tree->rect.width / 2;
	b.rect.height = 30;
	b.text.alignm.vertical = ALIGNM_CENTER_V;
	b.text.x = 20;
	b.rect.color = Color(0.1f, 0.1f, 0.1f);
	b.event.leftDown = AddOnTree;
	b.event.hoverOn = HoverOnSceneTree;
	b.event.hoverOff = HoverOffSceneTree;
	b.event.scroll = ScrollTree;
	b.data = id;
	int idButton = GUI::AddChild(idSceneTree, b);

	Object* obj = ObjectManager::GetWithNumber<Object>(ObjectManager::Size<Object>() - 1);
	obj->SetData(idButton);

	b.rect.x = b.rect.width;
	b.rect.width -= 25;
	b.text.text = L"Model";
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

	Picking::SetPickObject(obj);
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