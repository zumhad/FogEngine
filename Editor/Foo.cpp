#include "Foo.h"

#include "Main.h"


void MyUpdate()
{
	static bool isMoveScene = false;
	if (Input::IsMouseDown(MOUSE_LEFT) && !Application::CursorInScene())
		isMoveScene = true;
	if (Input::IsMouseUp(MOUSE_LEFT))
		isMoveScene = false;
	//
	// MOVE SCENE FOR MOUSE //
	//
	/*if (Input::IsMousePress(MOUSE_LEFT) && isMoveScene)
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

	if (Input::IsKeyDown(KEY_T))
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

	if (Input::IsKeyDown(KEY_L))
	{
		float x = ((rand() % 10) - 5) * 2;
		float z = ((rand() % 10) - 5) * 2;
		Vector3 dir = Normalize(Vector3(x, -10, z));

		Cube cube;
		cube.scale = Vector3(10, 10, 10);
		cube.position = -dir * 100;
		DirectionalLight l;
		l.direction = dir;
		ObjectManager::Add(l);
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
	}
}

void Update()
{
	if (Camera::GetRotateX() > 3.14 / 2.0)
		Camera::SetRotationX(3.14 / 2.0);
	else if (Camera::GetRotateX() < -3.14 / 2.0f)
		Camera::SetRotationX(-3.14 / 2.0f);

	float mouseSpeed = 0.001f;
	float yaw = Input::GetMouseAxis(MOUSE_X) * mouseSpeed;
	float pitch = Input::GetMouseAxis(MOUSE_Y) * mouseSpeed;

	float zMove = Input::IsKeyPress(KEY_W) * 1.0f - Input::IsKeyPress(KEY_S) * 1.0f; zMove *= 30.0f;
	float xMove = Input::IsKeyPress(KEY_D) * 1.0f - Input::IsKeyPress(KEY_A) * 1.0f; xMove *= 30.0f;
	if (xMove || zMove) Camera::MoveLocal(xMove, 0.0f, zMove);

	Camera::Rotate(pitch, yaw, 0.0f);

	

	MyUpdate();
}

void foo1()
{
	Cube cube;

	cube.material.diffuse = Vector4(1, 1, 1, 1);
	cube.position = Vector3((rand() % 10 - 5) * 2, 1, (rand() % 10 - 5) * 2);
	ObjectManager::Add(cube);
}

void foo2()
{
	Application::Exit();
}

void Start()
{
	Button b;

	b.x = 1000;
	b.y = 50;
	b.width = 200;
	b.height = 50;
	b.action = foo1;
	GUI::Add(b);
	
	b.x = Singlton.editor.width - 50;
	b.y = 0;
	b.width = 50;
	b.height = 50;
	b.action = foo2;
	b.color = Vector4(1, 0, 0, 1);
	GUI::Add(b);

	PointLight l;
	l.position = { 7, 5, 0 };
	ObjectManager::Add(l);

	Plane p;
	p.scale = { 10, 1, 10 };
	ObjectManager::Add(p);

	p.position = { 20, 0, 0 };
	ObjectManager::Add(p);


	//MyStart();
}



void Setting()
{
	Singlton.isGame = false;
	Singlton.cursorShow = true;
	Singlton.editor.color = { 70, 70, 70 };
	Singlton.foo.start = Start;
	/*Singlton.game.width = 800;
	Singlton.game.height = 600;
	Singlton.resolution.width = 800;
	Singlton.resolution.height = 600;*/
	Singlton.fpsMax = 0;
	Singlton.foo.update = Update;
	Singlton.scene.height = 800;
	Singlton.camera.rotationSmooth = 500;
	Singlton.camera.moveSmooth = 1000;
}


