#include "Engine.h"

#include "Events.h"
#include "Update.h"

void MyUpdate()
{
	if (Input::Down(KEY_DELETE))
	{
		Object* obj = Picking::GetPickObject();

		if (obj)
		{
			int number = GUI::GetWithID(obj->GetData())->GetChildNumber();

			Button* tree = GUI::GetWithID(idSceneTree);

			int size = tree->GetChildCount();
			for (int i = number + 2; i < size; i++)
			{
				Button* b = tree->GetChildWithNumber(i);
				b->rect.y -= b->rect.height;
			}

			GUI::DeleteWithID(obj->GetData());
			GUI::DeleteWithID(obj->GetData() + 1);

			Button* scrollBar = tree->GetChildWithNumber(0);
			Button* scroll = scrollBar->GetChildWithNumber(0);

			int maxCount = tree->rect.height / 30;
			int treeCount = (tree->GetChildCount() - 1) / 2;

			if (treeCount > 1)
			{
				float offset = (1.0f / (treeCount)) * (scrollBar->rect.height - 6);
				int offsetCount = Math::Round((scroll->rect.y - 3) / offset);

				if ((scroll->rect.y + scroll->rect.height + 3) == tree->rect.height && (scroll->rect.y - 3) != 0)
				{
					for (int i = 1; i < size - 2; i++)
					{
						Button* b = tree->GetChildWithNumber(i);
						b->rect.y += b->rect.height;
					}

					offsetCount--;
				}

				scroll->rect.y = Math::Round(offset * offsetCount) + 3;
			}

			scroll->rect.height = Math::Round(Math::Min(float(maxCount) / float(treeCount), 1.0f) * (scrollBar->rect.height - 6));

			ObjectManager::DeleteWithID(obj->GetID());
			Picking::SetPickObject(0);

			GUI::GetWithID(idTransform)->enable = false;
		}
	}

	if (Input::Down(KEY_L))
	{
		PointLight light;
		light.SetPosition(Vector3(0.0f, 5.0f, 0.0f));
		light.SetScale(Vector3(0.2f, 0.2f, 0.2f));
		light.SetRadius(20.0f);
		light.SetPower(100.0f);
		int id = ObjectManager::Add(light);

		static Button* tree = GUI::GetWithID(idSceneTree);
		int size = tree->GetChildCount();

		int offsetY = 0;
		if (size > 1)
		{
			Button* child = tree->GetChildWithNumber(size - 1);
			offsetY = child->rect.y + 30;
		}

		Button b;
		b.text.text = L"Point Light";
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
		b.text.text = L"Point Light";
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

	if (Input::Down(KEY_K))
	{
		DirectionLight light;
		light.SetColor(Color(1.0f, 1.0f, 1.0f));
		light.SetRotation(Vector3(45.0f, 45.0f, 0.0f));
		light.SetScale(Vector3(0.2f, 0.2f, 0.2f));
		light.SetPower(2.2f);
		int id = ObjectManager::Add(light);

		static Button* tree = GUI::GetWithID(idSceneTree);
		int size = tree->GetChildCount();

		int offsetY = 0;
		if (size > 1)
		{
			Button* child = tree->GetChildWithNumber(size - 1);
			offsetY = child->rect.y + 30;
		}

		Button b;
		b.text.text = L"Direction Light";
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
		b.text.text = L"Direction Light";
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

	if (Input::Down(KEY_UP))
	{
		Application::SetCascadeResolution(Application::GetCascadeResolution() * 2);
	}

	if (Input::Down(KEY_DOWN))
	{
		Application::SetCascadeResolution(Application::GetCascadeResolution() / 2);
	}
}

void fpstest()
{
	static int count = 0;
	static bool test = true;
	static int fpsCount = 0;

	if (count == fpsCount)
	{
		if (test)
		{
			Application::SetFpsMax(3000);
			fpsCount = 6000;
		}
		else
		{
			Application::SetFpsMax(30);
			fpsCount = 60;
		}

		test = !test;
		count = 0;
	}
	count++;
}

void MoveObject()
{
	static Object* obj = 0;
	static Vector4 plane;
	static Vector3 pick1;
	static Vector3 pick2;

	static bool x = true;
	static bool y = false;
	static bool z = true;

	static bool xPlane = false;
	static bool yPlane = false;
	static bool zPlane = false;

	static int sign = 0;

	if (Input::Down(MOUSE_LEFT) && Application::CursorInScene())
	{
		Picking::Pick();
		obj = Picking::GetPickObject();

		if (obj)
		{
			Vector3 r = Camera::GetDirection();

			float angX = Math::Abs(Vector3::Angle(r, Vector3(1, 0, 0)) - 90.0f);
			float angY = Math::Abs(Vector3::Angle(r, Vector3(0, 1, 0)) - 90.0f);
			float angZ = Math::Abs(Vector3::Angle(r, Vector3(0, 0, 1)) - 90.0f);

			xPlane = false;
			yPlane = false;
			zPlane = false;

			if (x && !y && !z)
			{
				if (angY > angZ) yPlane = true;
				else zPlane = true;
			}
			if (!x && y && !z)
			{
				if (angX > angZ) xPlane = true;
				else zPlane = true;
			}
			if (!x && !y && z)
			{
				if (angX > angY) xPlane = true;
				else yPlane = true;
			}

			if (!x && y && z) xPlane = true;
			if (x && !y && z) yPlane = true;
			if (x && y && !z) zPlane = true;

			Vector3 pickPos = Picking::GetPickPosition();
			Vector3 dir = Cursor::GetDirection();

			if (xPlane)
			{
				float h = pickPos.x;
				plane = Vector4(1, 0, 0, -h);
				sign = Math::Sign(dir.x);
			}
			if (yPlane)
			{
				float h = pickPos.y;
				plane = Vector4(0, 1, 0, -h);
				sign = Math::Sign(dir.y);
			}
			if (zPlane)
			{
				float h = pickPos.z;
				plane = Vector4(0, 0, 1, -h);
				sign = Math::Sign(dir.z);
			}

			pick1 = RayCasting::RayCast(dir, Camera::GetPosition(), plane);
		}
	}

	if (Input::Press(MOUSE_LEFT))
	{
		if (obj)
		{
			Vector3 dir = Cursor::GetDirection();
			float limit = 0.0f;

			if (xPlane) limit = dir.x;
			if (yPlane) limit = dir.y;
			if (zPlane) limit = dir.z;

			pick2 = RayCasting::RayCast(dir, Camera::GetPosition(), plane);

			if (Math::Sign(limit) == sign && pick1 != pick2 && pick2 == pick2)
			{
				Model& model = (Model&)(*obj);

				if (x) model.Move(Vector3(pick2.x - pick1.x, 0.0f, 0.0f));
				if (y) model.Move(Vector3(0.0f, pick2.y - pick1.y, 0.0f));
				if (z) model.Move(Vector3(0.0f, 0.0f, pick2.z - pick1.z));

				pick1 = pick2;
			}
		}
	}

	if (Input::Up(MOUSE_LEFT))
	{
		obj = 0;
	}
}

void Update()
{
	//fpstest();

	float moveSpeed = 10.0f;
	float rotationSpeed = 0.2f;

	Vector3 rot = Camera::GetRotation();

	static bool isRotate = false;
	if (Input::Down(MOUSE_RIGHT) && Application::CursorInScene()) isRotate = true;
	if (Input::Up(MOUSE_RIGHT)) isRotate = false;
	if (isRotate)
	{
		rot.y += Input::GetAxis(MOUSE_X) * rotationSpeed;
		rot.x += Input::GetAxis(MOUSE_Y) * rotationSpeed;

		if (rot.x > 90.0f) rot.x = 90.0f;
		if (rot.x < -90.0f) rot.x = -90.0f;

		Camera::SetRotation(rot);
	}

	float moveZ = (Input::Press(KEY_W) - Input::Press(KEY_S)) * moveSpeed * Time::DeltaTime();
	float moveX = (Input::Press(KEY_D) - Input::Press(KEY_A)) * moveSpeed * Time::DeltaTime();

	Vector3 move = Vector3(moveX, 0.0f, moveZ);
	move = Vector3::Rotate(move, Quaternion::Euler(Vector3::ConvertToRadians(rot)));

	Camera::Move(move);

	MoveObject();

	Object* obj = Picking::GetPickObject();
	static Button* transform = GUI::GetWithID(idTransform);
	static Button* material = GUI::GetWithID(idMaterial);
	static Button* light = GUI::GetWithID(idLight);

	UpdateSelectObject(obj);

	if (obj)
	{
		transform->enable = true;
		UpdateTransform(transform, obj);

		if (obj->GetType() == TypeObject::Model)
		{
			material->enable = true;
			UpdateMaterial(material, ObjectManager::Get<Model>(obj));
		}
		else
		{
			material->enable = false;
		}

		if (obj->GetType() == TypeObject::DirectionLight)
		{
			light->enable = true;
			UpdateLight(light, ObjectManager::Get<DirectionLight>(obj));
		}
		else
		{
			light->enable = false;
		}
	}
	else
	{
		transform->enable = false;
		material->enable = false;
		light->enable = false;
	}

	static Object* currentObject = 0;

	if (currentObject != obj)
	{
		static Button* properties = GUI::GetWithID(idProperties);

		int size = properties->GetChildCount();
		for (int i = 1; i < size; i++)
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

		currentObject = obj;
	}

	static Button* fps = GUI::GetWithID(idFPS);
	fps->text.text = String::ToString(Time::GetFPS());

	MyUpdate();
}

void Start()
{
	Camera::SetPosition(Vector3(0.0f, 10.0f, -10.0f));
	Camera::SetRotation(Vector3(45.0f, 0.0f, 0.0f));
	Camera::SetNear(0.1f);
	Camera::SetFar(500.0f);

	Application::SetCascadeSplit(0, 10.0f);
	Application::SetCascadeSplit(1, 30.0f);
	Application::SetCascadeSplit(2, 40.0f);
	Application::SetCascadeSplit(3, 50.0f);

	Application::SetCascadeResolution(1024);
	Application::SetCascadeBias(0.002f);
	Application::SetCascadeBlend(0.1f);
	Application::SetNormalBias(0.4f);

	Application::SetOutlineWidth(5);
	Application::SetOutlineColor(Color(1.0f, 1.0f, 0.0f));

	if (Application::IsGame()) return;

	Button caption;
	caption.rect.width = Application::GetEditorWidth();
	caption.rect.height = Application::GetCaptionHeight();
	caption.rect.color = Color(0.05f, 0.05f, 0.05f);
	int idCaption = GUI::Add(caption);

	Button fps;
	fps.text.x = 10;
	fps.text.y = 10;
	fps.rect.width = 100;
	fps.rect.height = Application::GetCaptionHeight() - 10;
	fps.text.text = L"0";
	fps.rect.color = Color(0.05f, 0.05f, 0.05f);
	idFPS = GUI::Add(fps);

	Button close;
	close.rect.width = 40;
	close.rect.height = 40;
	close.rect.alignm = { ALIGNM_RIGHT, ALIGNM_TOP };
	close.rect.color = Color(0.05f, 0.05f, 0.05f);
	close.event.hoverOn = HoverOn;
	close.event.hoverOff = HoverOff;
	close.event.leftClick = Close;
	GUI::AddChild(idCaption, close);

	close.event.leftClick = Restore;
	close.rect.x -= close.rect.width;
	GUI::AddChild(idCaption, close);

	close.event.leftClick = Minimize;
	close.rect.x -= close.rect.width;
	GUI::AddChild(idCaption, close);

	Button add;
	add.rect.color = Color(0.2f, 0.2f, 0.2f);
	add.rect.alignm = { ALIGNM_LEFT, ALIGNM_BOTTOM };
	add.rect.x = 10;
	add.rect.y = -10;
	add.rect.width = 200;
	add.rect.height = 50;
	add.text.text = L"Add Box";
	add.text.alignm = { ALIGNM_CENTER_H, ALIGNM_CENTER_V };
	add.event.leftClick = AddBox;
	add.event.hoverOn = HoverOn;
	add.event.hoverOff = HoverOff;
	GUI::Add(add);

	add.event.leftClick = AddSphere;
	add.text.text = L"Add Sphere";
	add.rect.x += add.rect.width + 10;
	GUI::Add(add);

	add.event.leftClick = AddPlane;
	add.text.text = L"Add Plane";
	add.rect.x += add.rect.width + 10;
	GUI::Add(add);

	Button tree;
	tree.rect.alignm = { ALIGNM_RIGHT, ALIGNM_TOP };
	tree.rect.x = -10;
	tree.rect.y = Application::GetCaptionHeight() + 10;
	tree.rect.width = Application::GetEditorWidth() - Application::GetSceneWidth() - 30;
	tree.rect.height = 405;
	tree.rect.color = Color(0.15f, 0.15f, 0.15f);
	int idTree = GUI::Add(tree);

	Button sceneTreeText;
	sceneTreeText.rect.alignm = { ALIGNM_LEFT, ALIGNM_TOP };
	sceneTreeText.rect.color = Color(0.15f, 0.15f, 0.15f);
	sceneTreeText.rect.x = 5;
	sceneTreeText.rect.y = 5;
	sceneTreeText.rect.width = 180;
	sceneTreeText.rect.height = 30;
	sceneTreeText.text.text = L"Scene Tree";
	sceneTreeText.text.x = 10;
	sceneTreeText.text.alignm = { ALIGNM_LEFT, ALIGNM_CENTER_V };
	GUI::AddChild(idTree, sceneTreeText);

	sceneTreeText.rect.color = Color(0.2f, 0.2f, 0.2f);
	sceneTreeText.rect.y += sceneTreeText.rect.height + 5;
	sceneTreeText.text.text = L"Name";
	sceneTreeText.text.x += 10;
	sceneTreeText.rect.width = tree.rect.width / 2;
	GUI::AddChild(idTree, sceneTreeText);

	sceneTreeText.rect.width -= 5;
	sceneTreeText.rect.color = Color(0.2f, 0.2f, 0.2f);
	sceneTreeText.rect.x += sceneTreeText.rect.width;
	sceneTreeText.text.text = L"Type";
	GUI::AddChild(idTree, sceneTreeText);

	Button sceneTree;
	sceneTree.rect.y = sceneTreeText.rect.y + sceneTreeText.rect.height;
	sceneTree.rect.width = tree.rect.width;
	sceneTree.rect.height = tree.rect.height - sceneTreeText.rect.height - sceneTreeText.rect.y - 5;
	sceneTree.rect.color = Color(0.15f, 0.15f, 0.15f);
	idSceneTree = GUI::AddChild(idTree, sceneTree);

	Button scroll;
	scroll.rect.x = -5;
	scroll.rect.alignm = { ALIGNM_RIGHT, ALIGNM_TOP };
	scroll.rect.width = 20;
	scroll.rect.height = sceneTree.rect.height;
	scroll.rect.color = Color(0.25f, 0.25f, 0.25f);
	scroll.event.scroll = ScrollTree;
	int idScroll = GUI::AddChild(idSceneTree, scroll);

	scroll.rect.alignm = { ALIGNM_LEFT, ALIGNM_TOP };
	scroll.rect.x = 3;
	scroll.rect.y = 3;
	scroll.rect.width = scroll.rect.width - 6;
	scroll.rect.height = scroll.rect.height - 6;
	scroll.rect.color = Color(0.35f, 0.35f, 0.35f);
	scroll.event.scroll = ScrollOnScroll;
	scroll.event.leftPress = PressScroll;
	scroll.event.leftUp = UpScroll;
	scroll.event.hoverOn = HoverOnScroll;
	scroll.event.hover = HoverScroll;
	scroll.event.hoverOff = HoverOffScroll;
	GUI::AddChild(idScroll, scroll);

	Button properties;
	properties.rect.alignm = { ALIGNM_RIGHT, ALIGNM_TOP };
	properties.rect.x = -10;
	properties.rect.y = tree.rect.y + tree.rect.height + 10;
	properties.rect.width = Application::GetEditorWidth() - Application::GetSceneWidth() - 30;
	properties.rect.height = Application::GetEditorHeight() - properties.rect.y - 10;
	properties.rect.color = Color(0.15f, 0.15f, 0.15f);
	idProperties = GUI::Add(properties);

	Button propertiesText;
	propertiesText.rect.alignm = { ALIGNM_LEFT, ALIGNM_TOP };
	propertiesText.rect.color = Color(0.15f, 0.15f, 0.15f);
	propertiesText.rect.x = 5;
	propertiesText.rect.y = 5;
	propertiesText.rect.width = 180;
	propertiesText.rect.height = 30;
	propertiesText.text.text = L"Properties";
	propertiesText.text.x = 10;
	propertiesText.text.alignm = { ALIGNM_LEFT, ALIGNM_CENTER_V };
	GUI::AddChild(idProperties, propertiesText);

	Button transform;
	transform.rect.alignm = { ALIGNM_LEFT, ALIGNM_TOP };
	transform.rect.color = Color(0.2f, 0.2f, 0.2f);
	transform.rect.x = 5;
	transform.rect.y = propertiesText.rect.y + propertiesText.rect.height + 5;
	transform.rect.width = properties.rect.width - 10;
	transform.rect.height = 140;
	transform.enable = false;
	idTransform = GUI::AddChild(idProperties, transform);

	Button transformText;
	transformText.rect.alignm = { ALIGNM_LEFT, ALIGNM_TOP };
	transformText.rect.color = Color(0.2f, 0.2f, 0.2f);
	transformText.rect.x = 0;
	transformText.rect.y = 0;
	transformText.rect.width = transform.rect.width;
	transformText.rect.height = 30;
	transformText.text.text = L"Transform";
	transformText.text.alignm = { ALIGNM_LEFT, ALIGNM_CENTER_V };
	transformText.text.x = 20;
	transformText.event.hoverOn = HoverOn;
	transformText.event.hoverOff = HoverOff;
	transformText.event.leftClick = PropertiesClick;

	GUI::AddChild(idTransform, transformText);

	Button position;
	position.rect.alignm = { ALIGNM_LEFT, ALIGNM_TOP };
	position.rect.color = Color(0.1f, 0.1f, 0.1f);
	position.rect.x = 5;
	position.rect.y = transformText.rect.y + transformText.rect.height + 5;
	position.rect.width = 167;
	position.rect.height = 30;
	position.text.text = L"Position";
	position.text.alignm = { ALIGNM_LEFT, ALIGNM_CENTER_V };
	position.text.x = 20;
	GUI::AddChild(idTransform, position);

	position.text.text = L"0.0";
	position.rect.x += position.rect.width + 5;
	position.rect.width = 136;
	position.event.leftPress = SliderPosX;
	GUI::AddChild(idTransform, position);

	position.text.text = L"0.0";
	position.rect.x += position.rect.width + 5;
	position.event.leftPress = SliderPosY;
	GUI::AddChild(idTransform, position);

	position.text.text = L"0.0";
	position.rect.x += position.rect.width + 5;
	position.event.leftPress = SliderPosZ;
	GUI::AddChild(idTransform, position);

	position.rect.alignm = { ALIGNM_LEFT, ALIGNM_TOP };
	position.rect.color = Color(0.1f, 0.1f, 0.1f);
	position.rect.x = 5;
	position.rect.y = position.rect.y + position.rect.height + 5;
	position.rect.width = 167;
	position.rect.height = 30;
	position.text.text = L"Rotation";
	position.text.alignm = { ALIGNM_LEFT, ALIGNM_CENTER_V };
	position.text.x = 20;
	GUI::AddChild(idTransform, position);

	position.text.text = L"0.0";
	position.rect.x += position.rect.width + 5;
	position.rect.width = 136;
	position.event.leftPress = SliderRotX;
	GUI::AddChild(idTransform, position);

	position.text.text = L"0.0";
	position.rect.x += position.rect.width + 5;
	position.event.leftPress = SliderRotY;
	GUI::AddChild(idTransform, position);

	position.text.text = L"0.0";
	position.rect.x += position.rect.width + 5;
	position.event.leftPress = SliderRotZ;
	GUI::AddChild(idTransform, position);


	position.rect.alignm = { ALIGNM_LEFT, ALIGNM_TOP };
	position.rect.color = Color(0.1f, 0.1f, 0.1f);
	position.rect.x = 5;
	position.rect.y = position.rect.y + position.rect.height + 5;
	position.rect.width = 167;
	position.rect.height = 30;
	position.text.text = L"Scale";
	position.text.alignm = { ALIGNM_LEFT, ALIGNM_CENTER_V };
	position.text.x = 20;
	GUI::AddChild(idTransform, position);

	position.text.text = L"0.0";
	position.rect.x += position.rect.width + 5;
	position.rect.width = 136;
	position.event.leftPress = SliderScaleX;
	GUI::AddChild(idTransform, position);

	position.text.text = L"0.0";
	position.rect.x += position.rect.width + 5;
	position.event.leftPress = SliderScaleY;
	GUI::AddChild(idTransform, position);

	position.text.text = L"0.0";
	position.rect.x += position.rect.width + 5;
	position.event.leftPress = SliderScaleZ;
	GUI::AddChild(idTransform, position);

	Button material;
	material.rect.alignm = { ALIGNM_LEFT, ALIGNM_TOP };
	material.rect.color = Color(0.2f, 0.2f, 0.2f);
	material.rect.x = 5;
	material.rect.y = transform.rect.y + transform.rect.height + 5;
	material.rect.width = properties.rect.width - 10;
	material.rect.height = 140;
	material.enable = false;
	idMaterial = GUI::AddChild(idProperties, material);

	Button materialText;
	materialText.rect.alignm = { ALIGNM_LEFT, ALIGNM_TOP };
	materialText.rect.color = Color(0.2f, 0.2f, 0.2f);
	materialText.rect.x = 0;
	materialText.rect.y = 0;
	materialText.rect.width = transform.rect.width;
	materialText.rect.height = 30;
	materialText.text.text = L"Material";
	materialText.text.alignm = { ALIGNM_LEFT, ALIGNM_CENTER_V };
	materialText.text.x = 20;
	materialText.event.hoverOn = HoverOn;
	materialText.event.hoverOff = HoverOff;
	materialText.event.leftClick = PropertiesClick;
	GUI::AddChild(idMaterial, materialText);

	Button color;
	color.rect.alignm = { ALIGNM_LEFT, ALIGNM_TOP };
	color.rect.x = 5;
	color.rect.y = materialText.rect.height + 5;
	color.rect.width = 167;
	color.rect.height = 30;
	color.text.text = L"Color";
	color.rect.color = Color(0.1f, 0.1f, 0.1f);
	color.text.alignm = { ALIGNM_LEFT, ALIGNM_CENTER_V };
	color.text.x = 20;
	GUI::AddChild(idMaterial, color);

	color.rect.x += color.rect.width + 5;
	color.text.text = L"0.0";
	color.rect.width = 136;
	color.event.leftPress = SliderColorR;
	GUI::AddChild(idMaterial, color);

	color.rect.x += color.rect.width + 5;
	color.text.text = L"0.0";
	color.event.leftPress = SliderColorG;
	GUI::AddChild(idMaterial, color);

	color.rect.x += color.rect.width + 5;
	color.text.text = L"0.0";
	color.event.leftPress = SliderColorB;
	GUI::AddChild(idMaterial, color);


	Button metallic;
	metallic.rect.alignm = { ALIGNM_LEFT, ALIGNM_TOP };
	metallic.rect.x = 5;
	metallic.rect.y = color.rect.y + color.rect.height + 5;
	metallic.rect.width = 167;
	metallic.rect.height = 30;
	metallic.text.text = L"Metallic";
	metallic.rect.color = Color(0.1f, 0.1f, 0.1f);
	metallic.text.alignm = { ALIGNM_LEFT, ALIGNM_CENTER_V };
	metallic.text.x = 20;
	GUI::AddChild(idMaterial, metallic);

	metallic.rect.x += metallic.rect.width + 5;
	metallic.text.text = L"0.0";
	metallic.rect.width = 136;
	metallic.event.leftPress = SliderMetallic;
	GUI::AddChild(idMaterial, metallic);

	Button roughness;
	roughness.rect.alignm = { ALIGNM_LEFT, ALIGNM_TOP };
	roughness.rect.x = 5;
	roughness.rect.y = metallic.rect.y + metallic.rect.height + 5;
	roughness.rect.width = 167;
	roughness.rect.height = 30;
	roughness.text.text = L"Roughness";
	roughness.rect.color = Color(0.1f, 0.1f, 0.1f);
	roughness.text.alignm = { ALIGNM_LEFT, ALIGNM_CENTER_V };
	roughness.text.x = 20;
	GUI::AddChild(idMaterial, roughness);

	roughness.rect.x += roughness.rect.width + 5;
	roughness.text.text = L"0.0";
	roughness.rect.width = 136;
	roughness.event.leftPress = SliderRoughness;
	GUI::AddChild(idMaterial, roughness);

	Button light;
	light.rect.alignm = { ALIGNM_LEFT, ALIGNM_TOP };
	light.rect.color = Color(0.2f, 0.2f, 0.2f);
	light.rect.x = 5;
	light.rect.y = transform.rect.y + transform.rect.height + 5;
	light.rect.width = properties.rect.width - 10;
	light.rect.height = 140;
	light.enable = false;
	idLight = GUI::AddChild(idProperties, light);

	Button lightText;
	lightText.rect.alignm = { ALIGNM_LEFT, ALIGNM_TOP };
	lightText.rect.color = Color(0.2f, 0.2f, 0.2f);
	lightText.rect.x = 0;
	lightText.rect.y = 0;
	lightText.rect.width = transform.rect.width;
	lightText.rect.height = 30;
	lightText.text.text = L"Light";
	lightText.text.alignm = { ALIGNM_LEFT, ALIGNM_CENTER_V };
	lightText.text.x = 20;
	lightText.event.hoverOn = HoverOn;
	lightText.event.hoverOff = HoverOff;
	lightText.event.leftClick = PropertiesClick;
	GUI::AddChild(idLight, lightText);

	Button colorLight;
	colorLight.rect.alignm = { ALIGNM_LEFT, ALIGNM_TOP };
	colorLight.rect.x = 5;
	colorLight.rect.y = lightText.rect.y + lightText.rect.height + 5;
	colorLight.rect.width = 167;
	colorLight.rect.height = 30;
	colorLight.text.text = L"Color";
	colorLight.rect.color = Color(0.1f, 0.1f, 0.1f);
	colorLight.text.alignm = { ALIGNM_LEFT, ALIGNM_CENTER_V };
	colorLight.text.x = 20;
	GUI::AddChild(idLight, colorLight);

	colorLight.rect.x += colorLight.rect.width + 5;
	colorLight.text.text = L"0.0";
	colorLight.rect.width = 136;
	colorLight.event.leftPress = SliderLightColorR;
	GUI::AddChild(idLight, colorLight);

	colorLight.rect.x += colorLight.rect.width + 5;
	colorLight.text.text = L"0.0";
	colorLight.event.leftPress = SliderLightColorG;
	GUI::AddChild(idLight, colorLight);

	colorLight.rect.x += colorLight.rect.width + 5;
	colorLight.text.text = L"0.0";
	colorLight.event.leftPress = SliderLightColorB;
	GUI::AddChild(idLight, colorLight);

	Button power;
	power.rect.alignm = { ALIGNM_LEFT, ALIGNM_TOP };
	power.rect.x = 5;
	power.rect.y = colorLight.rect.y + colorLight.rect.height + 5;
	power.rect.width = 167;
	power.rect.height = 30;
	power.text.text = L"Power";
	power.rect.color = Color(0.1f, 0.1f, 0.1f);
	power.text.alignm = { ALIGNM_LEFT, ALIGNM_CENTER_V };
	power.text.x = 20;
	GUI::AddChild(idLight, power);

	power.rect.x += power.rect.width + 5;
	power.text.text = L"0.0";
	power.rect.width = 136;
	power.event.leftPress = SliderPower;
	GUI::AddChild(idLight, power);

	AddRoom();
}

APPCLASS Setting()
{
	APPCLASS app;

	app.captionHeight = 50;
	app.isGame = false;
	app.cursorShow = true;
	app.foo.start = Start;
	app.foo.update = Update;
	app.fpsMax = 0;

	app.game.width = 1920;
	app.game.height = 1080;
	app.game.color = Color(1.0f, 1.0f, 1.0f);

	app.editor.color = Color(0.1f, 0.1f, 0.1f);

	app.scene.x = 10;
	app.scene.y = app.captionHeight + 10;
	app.scene.width = 1280;
	app.scene.height = 720;
	app.scene.color = Color(0.3f, 0.4f, 0.9f);

	return app;
}

CREATE_APPLICATION(Setting)