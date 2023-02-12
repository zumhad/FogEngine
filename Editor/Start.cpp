#include "Engine.h"

void MyUpdate()
{
	if (Input::Down(KEY_ESCAPE))
	{
		Application::Close();
	}

	if (Input::Down(KEY_Z))
	{
		Mesh m1;
		m1.name = L"sphere.obj";
		ObjectManager::Add(m1);
	}

	if (Input::Down(KEY_X))
	{
		Mesh m1;
		m1.name = L"box.obj";
		ObjectManager::Add(m1);
	}

	if (Input::Down(KEY_C))
	{
		Mesh m1;
		m1.name = L"plane.obj";
		ObjectManager::Add(m1);
	}

	if (Input::Down(KEY_L))
	{
		PointLight light;
		light.position.y = 5;
		light.range = 500.0f;
		light.power = 30.0f;
		ObjectManager::Add(light);
	}

	if (ObjectManager::Size() <= 0) return;

	Object& obj = ObjectManager::Get(ObjectManager::Size() - 1);

	if (obj.GetType() == TypeObject::Mesh)
	{
		Mesh& mesh = (Mesh&)obj;

		mesh.position.x += Input::Down(KEY_RIGHT) - Input::Down(KEY_LEFT);
	}
	else if (obj.GetType() == TypeObject::PointLight)
	{
		PointLight& light = (PointLight&)obj;

		light.position.x += Input::Down(KEY_RIGHT) - Input::Down(KEY_LEFT);
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

	static Vector3 pos = Vector3(0, 0, -50);
	static Vector3 rot = Camera::GetRotation();

	float move = (float)Input::GetAxis(MOUSE_SCROLL);
	move *= moveSpeed;

	pos.z += move;

	static bool isMove = false;
	if (Input::Down(MOUSE_LEFT) && Application::CursorInScene())
	{
		isMove = true;
	}

	if (!Input::Press(MOUSE_LEFT)) isMove = false;

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

	Object* obj = ObjectManager::GetSelectObject();

	if (obj && Input::Press(MOUSE_RIGHT))
	{
		Mesh& mesh = (Mesh&)(*obj);

		mesh.material.diffuse = Color(1, 0, 0);
	}

	MyUpdate();
}

void AddBox()
{
	Mesh m;
	m.name = L"box.obj";
	ObjectManager::Add(m);
}

void Start()
{
	Camera::SetRotationX(45);

	if (Application::IsGame()) return;

	Static s;
	s.x = 0;
	s.y = 0;
	s.width = Application::GetEditorWidth();
	s.height = Application::GetCaptionHeight();
	s.color = Color(0.7, 0.7, 0.7);
	GUI::Add(s);

	int size = 40;
	Button but;

	but.x = 0;
	but.y = 0;
	but.width = size;
	but.height = size;
	but.color = Color(0.5, 0.5, 0.5);
	but.focus = Color(1.0, 0.7, 0.7);
	but.action = Application::Close;
	but.alignm.horizontal = ALIGNM_RIGHT;
	GUI::Add(but);

	but.x += size;
	but.color = Color(0.5, 0.5, 0.5);
	but.action = Application::Restore;
	GUI::Add(but);

	but.x += size;
	but.color = Color(0.5, 0.5, 0.5);
	but.action = Application::Minimize;
	GUI::Add(but);

	but.x = 10;
	but.y = 60;
	but.width = 200;
	but.height = 50;
	but.color = Color(0.5, 0.5, 0.5);
	but.action = AddBox;
	GUI::Add(but);

	for (int i = -20; i < 20; i++)
	{
		for (int j = -20; j < 20; j++)
		{
			Mesh m1;
			m1.name = L"box.obj";
			m1.position = Vector3(i, 0, j);
			ObjectManager::Add(m1);
		}
	}
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

	app.game.width = 1920;
	app.game.height = 1080;
	app.game.color = Color(0, 0, 0);

	app.editor.color = Color(1, 1, 1);

	app.scene.x = 0;
	app.scene.y = 60;
	app.scene.width = 800;
	app.scene.height = 600;
	app.scene.color = Color(0.7f, 0.7f, 0.7f);

	return app;
}

CREATE_APPLICATION(Setting)