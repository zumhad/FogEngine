#include "Foo.h"
#include "Main.h"


void MyUpdate()
{
	//static bool isMoveScene = false;
	//if (Input::IsMouseDown(MOUSE_LEFT) && !Application::CursorInScene())
	//	isMoveScene = true;
	//if (Input::IsMouseUp(MOUSE_LEFT))
	//	isMoveScene = false;
	//
	// MOVE SCENE FOR MOUSE //
	//
	/**if (Input::IsMousePress(MOUSE_LEFT) && isMoveScene)
	{
		if (Input::GetCursorX() + Application::GetSceneWidth() <= Singlton.editor.width)
			Application::SetSceneX(Input::GetCursorX());
		else
			Application::SetSceneX(Singlton.editor.width - Application::GetSceneWidth());



		if (Input::GetCursorY() + Application::GetSceneHeight() > Singlton.editor.height)
			Application::SetSceneY(Singlton.editor.height - Application::GetSceneHeight());
		else if (Input::GetCursorY() < Singlton.captionHeight)
			Application::SetSceneY(Singlton.captionHeight);
		else
			Application::SetSceneY(Input::GetCursorY());
	}*/

	/*if (Input::IsKeyDown(KEY_T))
	{
		ObjectManager::Clear();
	}

	if (Input::IsKeyDown(KEY_R))
	{
		Cube cube;

		cube.material.diffuse = Vector4(1, 1, 1, 1);
		cube.position = Vector3((rand() % 10 - 5) * 2, 1, (rand() % 10 - 5) * 2);
		ObjectManager::Add(cube);
	}

	if (Input::IsKeyPress(KEY_CTRL) && Input::IsKeyDown(KEY_5))
	{
		Application::SaveProject();
	}

	if (Input::IsKeyDown(KEY_ESC))
	{
		Application::SaveProject();
		Application::Exit();
	}

	if (Input::IsKeyDown(KEY_Q))
	{
		Application::RestartShader();
	}*/

	if (Input::Down(KEY_ESCAPE))
	{
		Application::Exit();
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
		light.range = 10.0f;
		light.power = 3.0f;
		ObjectManager::Add(light);
	}

	if (ObjectManager::Size() <= 0) return;

	Object& obj = ObjectManager::Get(ObjectManager::Size()-1);

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
			Singlton.fpsMax = 300;
		}
		else
		{
			Singlton.fpsMax = 50;
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

	static Vector3 pos = Vector3(0, 0, -5);
	static Vector3 rot = Camera::GetRotation();

	float move = (float)Input::GetAxis(MOUSE_SCROLL);
	move *= moveSpeed;

	pos.z += move;

	static bool isMove = false;
	if (Input::Down(MOUSE_LEFT) && Application::CursorInScene())
	{
		isMove = true;
	}

	if (Input::Up(MOUSE_LEFT)) isMove = false;

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

	MyUpdate();
}

void exit()
{
	Application::Exit();
}

void Start()
{
	Camera::SetRotationX(45);

	Button but;
	but.x = Singlton.editor.width - 50;
	but.y = 0;
	but.width = 50;
	but.height = 50;
	but.action = exit;
	but.color = Color(1, 0, 0);
	GUI::Add(but);
}



void Setting()
{
	Singlton.isGame = false;
	Singlton.cursorShow = true; 
	Singlton.scene.color = Color(0.7f, 0.7f, 0.7f);
	Singlton.editor.color = Color(1, 1, 1);
	Singlton.foo.start = Start;
	Singlton.foo.update = Update;
	Singlton.scene.y = 60;
	//Singlton.fpsMax = 50;
	Singlton.camera.rotationSmooth = 500;
	Singlton.camera.moveSmooth = 1000;
}