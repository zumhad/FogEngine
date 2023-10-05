#include "Engine.h"

int idFPS = -1;
int idPos = -1;

void MyUpdate()
{
	if (Input::Down(KEY_ESCAPE))
	{
		Application::Close();
	}

	if (Input::Down(KEY_L))
	{
		PointLight light;
		light.position.y = 100;
		light.position.z = 2;
		light.radius = 10.0f;
		light.power = 1000.0f;
		ObjectManager::Add(light);

		Mesh m;
		m.name = L"sphere.obj";
		m.position = Vector3(0.0, 10, 2.0);
		m.scale = Vector3(1.0, 1.0, 1.0);
		m.material.diffuse = Color(1.0f, 0.0f, 0.0f);
		ObjectManager::Add(m);
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
	float moveSpeed = 0.01f;
	float rotationSpeed = 50.0f;

	static Vector3 pos = Camera::GetPosition();
	static Vector3 rot = Camera::GetRotation();

	static bool isRotate = false;
	if (Input::Down(MOUSE_RIGHT) && Application::CursorInScene()) isRotate = true;
	if (Input::Up(MOUSE_RIGHT)) isRotate = false;
	if (isRotate)
	{
		rot.y += Input::GetAxis(MOUSE_X) * rotationSpeed;
		rot.x += Input::GetAxis(MOUSE_Y) * rotationSpeed;

		if (rot.x > 89.9f) rot.x = 89.9f;
		if (rot.x < -89.9f) rot.x = -89.9f;

		Camera::SetRotation(rot);
	}

	float moveZ = (Input::Press(KEY_W) - Input::Press(KEY_S)) * moveSpeed;
	float moveX = (Input::Press(KEY_D) - Input::Press(KEY_A)) * moveSpeed;

	Vector3 move = Vector3(moveX, 0.0f, moveZ);
	move = move.Rotate(move, Quaternion::Euler(rot.x, rot.y, 0.0f));
	Camera::Move(Vector3(move.x, move.y, move.z));

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
			Vector3 camPos = Camera::GetPosition();
			Vector3 dir = pickPos - camPos;

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

			pick1 = RayCasting::RayCast(dir, camPos, plane);

			Vector3 test = RayCasting::RayCast(Vector3(0, -1, 1), Vector3(0, 5, -5), Vector4(0, 1, 0, 0));
		}
	}

	if (Input::Press(MOUSE_LEFT))
	{
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

					Text& t = (Text&)GUI::Get(idPos);
					t.text = str;
				}
			}
		}
		else
		{
			String str = L"";
			str += L"x: " + String::ToString(0.0f) + L"\n";
			str += L"y: " + String::ToString(0.0f) + L"\n";
			str += L"z: " + String::ToString(0.0f);

			Text& t = (Text&)GUI::Get(idPos);
			t.text = str;
		}
	}

	if (!Application::IsGame())
	{
		Text& t = (Text&)GUI::Get(idFPS);
		t.text = String::ToString(Time::GetFPS());
	}

	MyUpdate();
}

void AddBox()
{
	Mesh m;
	m.name = L"box.obj";
	ObjectManager::Add(m);
}

void AddPlane()
{
	Mesh m;
	m.name = L"plane.obj";
	m.scale = Vector3(100, 1, 100);
	ObjectManager::Add(m);
}

void AddSphere()
{
	Mesh m;
	m.name = L"sphere.obj";
	ObjectManager::Add(m);
}

void Start()
{
	DirectionalLight dir;
	dir.color = Color(1, 1, 1);
	dir.direction = Vector3(0.0f, 0.0f, -1.0f);
	//ObjectManager::Add(dir);

	Mesh m;
	m.name = L"house.obj";
	m.texture = L"png.png";
	m.position = Vector3(0.0, 0.0, 0.0);
	m.scale = Vector3(1.0, 1.0, 1.0);
	m.rotation = Vector3(0, 0, 0);
	m.material.diffuse = Color(1.0f, 1.0f, 1.0f);
	ObjectManager::Add(m);

	Camera::SetPosition(Vector3(0, 2, -10));
	Camera::SetRotationX(0.0f);
	Camera::SetFar(0.1f);
	Camera::SetNear(10000.0f);

	if (Application::IsGame()) return;

	Static s;
	s.x = 0;
	s.y = 0;
	s.width = Application::GetEditorWidth();
	s.height = Application::GetCaptionHeight();
	s.color = Color(0.7f, 0.7f, 0.7f);
	int idCaption = GUI::Add(s);

	s.x = 0;
	s.y = Application::GetCaptionHeight() + 10;
	s.width = 200;
	s.height = 100;
	s.alignm.horizontal = ALIGNM_RIGHT;
	s.alignm.vertical = ALIGNM_TOP;
	s.color = Color(0.7f, 0.7f, 0.7f);
	int infoRect = GUI::Add(s);

	int size = 40;
	Button but;

	but.x = 0;
	but.y = 0;
	but.width = size;
	but.height = size;
	but.color = Color(0.5f, 0.5f, 0.5f);
	but.focus = Color(1.0f, 0.7f, 0.7f);
	but.action = Application::Close;
	but.alignm.horizontal = ALIGNM_RIGHT;
	GUI::Add(but);

	but.x -= size;
	but.color = Color(0.5f, 0.5f, 0.5f);
	but.action = Application::Restore;
	GUI::Add(but);

	but.x -= size;
	but.color = Color(0.5f, 0.5f, 0.5f);
	but.action = Application::Minimize;
	GUI::Add(but);

	but.x = 10;
	but.y = -10;
	but.width = 200;
	but.height = 50;
	but.color = Color(0.5f, 0.5f, 0.5f);
	but.action = AddBox;
	but.alignm.horizontal = ALIGNM_LEFT;
	but.alignm.vertical = ALIGNM_BOTTOM;
	int idAddBox = GUI::Add(but);

	but.x += 210;
	but.action = AddSphere;
	int idAddSphere = GUI::Add(but);

	but.x += 210;
	but.action = AddPlane;
	int idAddPlane = GUI::Add(but);

	Text t;
	t.text = L"";
	t.x = 10;
	t.alignm.horizontal = ALIGNM_LEFT;
	t.alignm.vertical = ALIGNM_TOP;
	idFPS = GUI::AddChild(idCaption, t);

	t.x = 10;
	t.text = L"x: " + String::ToString(0.0f) + L"\ny: " + String::ToString(0.0f) + L"\nz: " + String::ToString(0.0f);
	t.alignm.horizontal = ALIGNM_LEFT;
	t.alignm.vertical = ALIGNM_CENTER_V;
	idPos = GUI::AddChild(infoRect, t);

	t.x = 0;
	t.alignm.horizontal = ALIGNM_CENTER_H;
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
	app.camera.rotationSmooth = 500;
	app.camera.moveSmooth = 10;
	app.fpsMax = 0;

	app.game.width = 1920;
	app.game.height = 1080;
	app.game.color = Color(1.0f, 1.0f, 1.0f);

	app.editor.color = Color(1.0f, 1.0f, 1.0f);

	app.scene.x = 0;
	app.scene.y = app.captionHeight + 10;
	app.scene.width = 1200;
	app.scene.height = 800;
	app.scene.color = Color(0.7f, 0.7f, 0.7f);

	return app;
}

CREATE_APPLICATION(Setting)