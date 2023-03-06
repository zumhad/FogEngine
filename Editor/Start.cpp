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
		light.position.y = 40;
		light.range = 50.0f;
		light.power = 15.0f;
		ObjectManager::Add(light);
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
	float moveSpeed = 1.0f;
	float rotationSpeed = 100.0f;

	static Vector3 pos = Vector3(0, 0, -10);
	static Vector3 rot = Camera::GetRotation();

	float move = (float)Input::GetAxis(MOUSE_SCROLL);
	move *= moveSpeed;

	pos.z += move;

	static bool isMove = false;
	if (Input::Down(MOUSE_RIGHT) && Application::CursorInScene())
	{
		isMove = true;
	}

	if (!Input::Press(MOUSE_RIGHT)) isMove = false;

	if (isMove)
	{
		rot.y += Input::GetAxis(MOUSE_X) * rotationSpeed;
		rot.x += Input::GetAxis(MOUSE_Y) * rotationSpeed;
	}

	if (rot.x > 89.9f) rot.x = 89.9f;
	if (rot.x < -89.9f) rot.x = -89.9f;

	Vector3 targetPos = Vector3::Rotate(pos, Quaternion::Euler(rot.x, rot.y, 0));

	Camera::SetPosition(targetPos);
	Camera::LookAt(Vector3::Zero());


	static Object* obj = 0;
	static Vector3 start;	

	if (Input::Down(MOUSE_LEFT) && Application::CursorInScene())
	{
		static Object* prevObj = 0;
		static Color prevColor;
		obj = ObjectManager::GetSelectObject();

		if (obj != prevObj)
		{
			if (prevObj)
			{
				Mesh& prevMesh = (Mesh&)(*prevObj);
				prevMesh.material.diffuse = prevColor;
			}
		}

		if (obj)
		{
			Mesh& mesh = (Mesh&)(*obj);

			if(obj != prevObj) prevColor = mesh.material.diffuse;

			mesh.material.diffuse = Color(1, 0, 0);

			start = Picking::Pick(Cursor::GetDirection(), Camera::GetPosition());
			start -= mesh.position;
		}

		if (obj != prevObj) prevObj = obj;
	}

	if (Input::Press(MOUSE_LEFT) && Application::CursorInScene())
	{
		if (obj)
		{
			Vector3 end = Picking::Pick(Cursor::GetDirection(), Camera::GetPosition());
			end -= start;

			Mesh& mesh = (Mesh&)(*obj);
			mesh.position.x = end.x;
			mesh.position.z = end.z;

			String str = L"";
			str += L"x: " + String::ToStr(mesh.position.x) + L"\n";
			str += L"y: " + String::ToStr(mesh.position.y) + L"\n";
			str += L"z: " + String::ToStr(mesh.position.z);

			Text & t = (Text&)GUI::Get(idPos);
			t.text = str;
		}
		else
		{
			String str = L"";
			str += L"x: " + String::ToStr(0.0f) + L"\n";
			str += L"y: " + String::ToStr(0.0f) + L"\n";
			str += L"z: " + String::ToStr(0.0f);

			Text& t = (Text&)GUI::Get(idPos);
			t.text = str;
		}
	}

	Text& t = (Text&)GUI::Get(idFPS);
	t.text = L"FPS: " + String::ToStr(Time::GetFPS());

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
	Mesh m;
	m.name = L"2107.obj";
	m.material.diffuse = Color(0.5f, 0.0f, 0.1f);
	ObjectManager::Add(m);

	Camera::SetRotationX(45);

	for (int i = -20; i < 20; i++)
	{
		for (int j = -20; j < 20; j++)
		{
			Mesh m1;
			m1.name = L"box.obj";
			m1.position = Vector3(float(i), 0.0f, float(j));
			ObjectManager::Add(m1);
		}
	}

	if (Application::IsGame()) return;

	Static s;
	s.x = 0;
	s.y = 0;
	s.width = Application::GetEditorWidth();
	s.height = Application::GetCaptionHeight();
	s.color = Color(0.7f, 0.7f, 0.7f);
	int idCaption = GUI::Add(s);

	s.x = 0;
	s.y = 100;
	s.width = 300;
	s.height = 150;
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
	t.alignm.vertical = ALIGNM_CENTER_V;
	idFPS = GUI::AddChild(idCaption, t);

	t.x = 0;
	t.text = L"x: 0.0\ny: 0.0\nz: 0.0";
	t.alignm.horizontal = ALIGNM_CENTER_H;
	t.alignm.vertical = ALIGNM_CENTER_V;
	idPos = GUI::AddChild(infoRect, t);

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
	app.captionHeight = 50;
	app.isGame = false;
	app.cursorShow = true;
	app.foo.start = Start;
	app.foo.update = Update;
	app.camera.rotationSmooth = 500;
	app.camera.moveSmooth = 1000;
	app.fpsMax = 0;

	app.game.width = 1920;
	app.game.height = 1080;
	app.game.color = Color(0.0f, 0.0f, 0.0f);

	app.editor.color = Color(1.0f, 1.0f, 1.0f);

	app.scene.x = 0;
	app.scene.y = 60;
	app.scene.width = 800;
	app.scene.height = 600;
	app.scene.color = Color(0.7f, 0.7f, 0.7f);

	return app;
}

CREATE_APPLICATION(Setting)