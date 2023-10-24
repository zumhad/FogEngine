#include "Engine.h"

#include "Events.h"

void MyUpdate()
{
	if (Input::Down(KEY_ESCAPE))
	{
		Application::Close();
	}

	if (Input::Down(KEY_L))
	{
		PointLight light;
		light.position.y = 10;
		light.position.z = 0;
		light.radius = 1.0f;
		light.power = 100.0f;
		ObjectManager::Add(light);

		Model m;
		m.name = L"sphere.obj";
		m.position = Vector3(0.0f, 1.0f, 0.0f);
		m.scale = Vector3(0.1f, 0.1f, 0.1f);
		m.color = Color(1.0f, 1.0f, 1.0f);
		m.texture = L"png.png";
		//ObjectManager::Add(m);
	}

	if (Input::Down(KEY_UP))
	{
		ShadowMap::SetResolution(ShadowMap::GetResolution() * 2);
	}

	if (Input::Down(KEY_DOWN))
	{
		ShadowMap::SetResolution(ShadowMap::GetResolution() / 2);
	}
}


void fpstest()
{
	static int count = 0;
	static bool test = true;

	if (count == 50)
	{
		if (test)
		{
			Application::SetFpsMax(300);
		}
		else
		{
			Application::SetFpsMax(50);
		}

		test = !test;
		count = 0;
	}
	count++;
}

void Update()
{
	float moveSpeed = 10.0f;
	float rotationSpeed = 50.0f;

	static Vector3 pos = Camera::GetPosition();
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
	move = move.Rotate(move, Quaternion::Euler(rot.x, rot.y, 0.0f));
	pos += move;

	Vector3 vel;
	Vector3 res = Vector3::SmoothDamp(Camera::GetPosition(), pos, vel, Time::DeltaTime() * 15.0f);
	Camera::SetPosition(res);

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

		Text* posX = GUI::Get<Text>(idPosX);
		Text* posY = GUI::Get<Text>(idPosY);
		Text* posZ = GUI::Get<Text>(idPosZ);

		GUI::GetParent<Static>(posX)->enable = false;
		GUI::GetParent<Static>(posY)->enable = false;
		GUI::GetParent<Static>(posZ)->enable = false;

		if (obj)
		{
			GUI::GetParent<Static>(posX)->enable = true;
			GUI::GetParent<Static>(posY)->enable = true;
			GUI::GetParent<Static>(posZ)->enable = true;

			posX->text = String::ToString(ObjectManager::Get<Model>(obj)->position.x);
			posY->text = String::ToString(ObjectManager::Get<Model>(obj)->position.y);
			posZ->text = String::ToString(ObjectManager::Get<Model>(obj)->position.z);

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

			Vector3 test = RayCasting::RayCast(Vector3(0, -1, 1), Vector3(0, 5, -5), Vector4(0, 1, 0, 0));
		}
	}

	if (Input::Press(MOUSE_LEFT))
	{
		Text* posX = GUI::Get<Text>(idPosX);
		Text* posY = GUI::Get<Text>(idPosY);
		Text* posZ = GUI::Get<Text>(idPosZ);

		if (obj)
		{
			if (Application::CursorInScene())
			{
				Vector3 dir = Cursor::GetDirection();
				float limit = 0.0f;

				if (xPlane) limit = dir.x;
				if (yPlane) limit = dir.y;
				if (zPlane) limit = dir.z;

				pick2 = RayCasting::RayCast(dir, Camera::GetPosition(), plane);

				if (Math::Sign(limit) == sign && pick1 != pick2)
				{
					Model& model = (Model&)(*obj);

					if (x) model.position.x += pick2.x - pick1.x;
					if (y) model.position.y += pick2.y - pick1.y;
					if (z) model.position.z += pick2.z - pick1.z;

					pick1 = pick2;

					String str = L"";
					str += L"x: " + String::ToString(model.position.x) + L"\n";
					str += L"y: " + String::ToString(model.position.y) + L"\n";
					str += L"z: " + String::ToString(model.position.z);

					posX->text = String::ToString(model.position.x);
					posY->text = String::ToString(model.position.y);
					posZ->text = String::ToString(model.position.z);
				}
			}
		}
	}

	if (Input::Up(MOUSE_LEFT))
	{
		obj = 0;
	}

	if (!Application::IsGame())
	{
		Text* text = GUI::Get<Text>(idFPS);

		if (text)
		{
			text->text = String::ToString(Time::GetFPS());
		}
	}

	MyUpdate();
}

void Start()
{
	DirectionalLight dir;
	dir.color = Color(1, 1, 1);
	dir.direction = Vector3(0.0f, -1.0f, -1.0f);
	dir.power = 1.2f;
	ObjectManager::Add(dir);

	Model m;
	m.name = L"room.obj";
	m.texture = L"png.png";
	m.position = Vector3(0.0, 0.0, 0.0);
	m.scale = Vector3(1.0, 1.0, 1.0);
	m.rotation = Vector3(0, 0, 0);
	m.color = Color(1.0f, 1.0f, 1.0f);
	ObjectManager::Add(m);

	Camera::SetPosition(Vector3(0, 5, -5));
	Camera::SetRotationX(0.0f);
	Camera::SetFar(0.001f);
	Camera::SetNear(1000.0f);

	if (Application::IsGame()) return;

	Static s;
	s.x = 0;
	s.y = 0;
	s.width = Application::GetEditorWidth();
	s.height = Application::GetCaptionHeight();
	s.color = Color(0.05f, 0.05f, 0.05f);
	int idCaption = GUI::Add(s);

	s.x = -10;
	s.y = Application::GetCaptionHeight() + 10;
	s.width = 460;
	s.height = Application::GetEditorHeight() - Application::GetCaptionHeight() - 20;
	s.alignm.horizontal = ALIGNM_RIGHT;
	s.alignm.vertical = ALIGNM_TOP;
	s.color = Color(0.18f, 0.18f, 0.18f);
	int infoRect = GUI::Add(s);

	Static pos;
	pos.x = 10;
	pos.y = 10;
	pos.width = 140;
	pos.height = 50;
	pos.color = Color(0.25, 0.25, 0.25);
	pos.event.focus = FocusX;
	pos.event.focusOn = FocusOnX;
	pos.event.focusOff = FocusOffX;
	pos.enable = false;
	int posX = GUI::AddChild(infoRect, pos);

	pos.x += pos.width + 10;
	pos.event.focus = FocusY;
	pos.event.focusOn = FocusOnY;
	pos.event.focusOff = FocusOffY;
	int posY = GUI::AddChild(infoRect, pos);

	pos.event.focus = FocusZ;
	pos.event.focusOn = FocusOnZ;
	pos.event.focusOff = FocusOffZ;
	pos.x += pos.width + 10;
	int posZ = GUI::AddChild(infoRect, pos);

	Text textBox;
	textBox.alignm.horizontal = ALIGNM_LEFT;
	textBox.alignm.vertical = ALIGNM_CENTER_V;
	textBox.x = 5;
	textBox.color = Color(1.0f, 1.0f, 1.0f);
	textBox.size = 16;
	idPosX = GUI::AddChild(posX, textBox);
	idPosY = GUI::AddChild(posY, textBox);
	idPosZ = GUI::AddChild(posZ, textBox);

	int size = 40;
	Static but;

	but.x = 0;
	but.y = 0;
	but.width = size;
	but.height = size;
	but.color = Color(0.05f, 0.05f, 0.05f);
	but.event.hoverOn = HoverOn;
	but.event.hoverOff = HoverOff;
	but.event.leftClick = Close;
	but.alignm.horizontal = ALIGNM_RIGHT;
	GUI::Add(but);

	but.x -= size;
	but.event.leftClick = Restore;
	GUI::Add(but);

	but.x -= size;
	but.event.leftClick = Minimize;
	GUI::Add(but);

	but.x = 10;
	but.y = -10;
	but.width = 200;
	but.height = 50;
	but.color = Color(0.05f, 0.05f, 0.05f);
	but.event.leftClick = AddBox;
	but.alignm.horizontal = ALIGNM_LEFT;
	but.alignm.vertical = ALIGNM_BOTTOM;
	int idAddBox = GUI::Add(but);

	but.x += 210;
	but.event.leftClick = AddSphere;
	int idAddSphere = GUI::Add(but);

	but.x += 210;
	but.event.leftClick = AddPlane;
	int idAddPlane = GUI::Add(but);

	Text t;
	t.text = L"";
	t.x = 10;
	t.color = Color(1.0f, 1.0f, 1.0f);
	t.alignm.horizontal = ALIGNM_LEFT;
	t.alignm.vertical = ALIGNM_TOP;
	idFPS = GUI::AddChild(idCaption, t);

	t.size = 16;
	t.x = 0;
	t.alignm.horizontal = ALIGNM_CENTER_H;
	t.alignm.vertical = ALIGNM_CENTER_V;
	t.text = L"AddBox";
	GUI::AddChild(idAddBox, t);

	t.text = L"AddSphere";
	GUI::AddChild(idAddSphere, t);

	t.text = L"AddPlane";
	GUI::AddChild(idAddPlane, t);
}

APPCLASS Setting()
{
	APPCLASS app;

	app.captionHeight = 100;
	app.isGame = false;
	app.cursorShow = true;
	app.foo.start = Start;
	app.foo.update = Update;
	app.camera.rotationSmooth = 50;
	app.camera.moveSmooth = 10;
	app.fpsMax = 0;

	app.game.width = 1920;
	app.game.height = 1080;
	app.game.color = Color(1.0f, 1.0f, 1.0f);

	app.editor.color = Color(0.1f, 0.1f, 0.1f);

	app.scene.x = 0;
	app.scene.y = app.captionHeight + 10;
	app.scene.width = 1440;
	app.scene.height = 800;
	app.scene.color = Color(0.3f, 0.4f, 0.9f);

	return app;
}

CREATE_APPLICATION(Setting)