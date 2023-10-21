#include "Engine.h"

#include "Events.h"

int idFPS = -1;
int idSliderX = -1;
int idSliderY = -1;
int idSliderZ = -1;

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

		Mesh m;
		m.name = L"sphere.obj";
		m.position = Vector3(0.0f, 1.0f, 0.0f);
		m.scale = Vector3(0.1f, 0.1f, 0.1f);
		m.material.diffuse = Color(1.0f, 1.0f, 1.0f);
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

		Text& sliderX = GUI::Get<Text>(idSliderX);
		Text& sliderY = GUI::Get<Text>(idSliderY);
		Text& sliderZ = GUI::Get<Text>(idSliderZ);

		sliderX.enable = false;
		sliderY.enable = false;
		sliderZ.enable = false;

		if (obj)
		{
			sliderX.enable = true;
			sliderY.enable = true;
			sliderZ.enable = true;

			sliderX.text = String::ToString(((Mesh&)(*obj)).position.x);
			sliderY.text = String::ToString(((Mesh&)(*obj)).position.y);
			sliderZ.text = String::ToString(((Mesh&)(*obj)).position.z);

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
		Text& sliderX = GUI::Get<Text>(idSliderX);
		Text& sliderY = GUI::Get<Text>(idSliderY);
		Text& sliderZ = GUI::Get<Text>(idSliderZ);

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
					Mesh& mesh = (Mesh&)(*obj);

					if (x) mesh.position.x += pick2.x - pick1.x;
					if (y) mesh.position.y += pick2.y - pick1.y;
					if (z) mesh.position.z += pick2.z - pick1.z;

					pick1 = pick2;

					String str = L"";
					str += L"x: " + String::ToString(mesh.position.x) + L"\n";
					str += L"y: " + String::ToString(mesh.position.y) + L"\n";
					str += L"z: " + String::ToString(mesh.position.z);

					sliderX.text = String::ToString(mesh.position.x);
					sliderY.text = String::ToString(mesh.position.y);
					sliderZ.text = String::ToString(mesh.position.z);
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
		Text& t = GUI::Get<Text>(idFPS);
		t.text = String::ToString(Time::GetFPS());
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

	Mesh m;
	m.name = L"house.obj";
	m.texture = L"png.png";
	m.position = Vector3(0.0, 0.0, 0.0);
	m.scale = Vector3(1.0, 1.0, 1.0);
	m.rotation = Vector3(0, 0, 0);
	m.material.diffuse = Color(1.0f, 1.0f, 1.0f);
	ObjectManager::Add(m);

	Camera::SetPosition(Vector3(0, 0, 0));
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
	int idPosX = GUI::AddChild(infoRect, pos);

	pos.x += pos.width + 10;
	int idPosY = GUI::AddChild(infoRect, pos);

	pos.x += pos.width + 10;
	int idPosZ = GUI::AddChild(infoRect, pos);

	Text textBox;
	textBox.alignm.horizontal = ALIGNM_LEFT;
	textBox.alignm.vertical = ALIGNM_CENTER_V;
	textBox.x = 5;
	textBox.event.focus = Focus;
	textBox.event.focusOn = FocusOn;
	textBox.event.focusOff = FocusOffX;
	textBox.enable = false;
	textBox.color = Color(1.0f, 1.0f, 1.0f);
	textBox.size = 16;
	idSliderX = GUI::AddChild(idPosX, textBox);

	//textBox.x += textBox.width + 10;
	textBox.event.focusOff = FocusOffY;
	idSliderY = GUI::AddChild(idPosY, textBox);

	//textBox.x += textBox.width + 10;
	textBox.event.focusOff = FocusOffZ;
	idSliderZ = GUI::AddChild(idPosZ, textBox);

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