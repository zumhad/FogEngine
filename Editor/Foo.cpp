#include "Foo.h"

#include "Main.h"


void MyUpdate()
{
	static bool isMoveScene = false;
	if (Input::IsMouseDown(MOUSE_LEFT) && !Application::CursorInScene())
		isMoveScene = true;
	if (Input::IsMouseUp(MOUSE_LEFT))
		isMoveScene = false;

	if (Input::IsMousePress(MOUSE_LEFT) && isMoveScene)
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
	}

	if (Input::IsKeyDown(KEY_T))
	{
		ObjectManager::Clear();
	}

	if (Input::IsKeyDown(KEY_R))
	{
		Cube cube;

		cube.material.diffuse = Vector4(1, 1, 1, 1);
		cube.position = Vector3((rand() % 10 - 5) * 2, 0, (rand() % 10 - 5) * 2);
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
	float mouseSpeed = 0.001f;
	float yaw = Input::GetMouseAxis(MOUSE_X) * mouseSpeed;
	float pitch = Input::GetMouseAxis(MOUSE_Y) * mouseSpeed;

	float zMove = Input::IsKeyPress(KEY_W) * 1.0f - Input::IsKeyPress(KEY_S) * 1.0f; zMove *= 30;
	float xMove = Input::IsKeyPress(KEY_D) * 1.0f - Input::IsKeyPress(KEY_A) * 1.0f; xMove *= 30;
	if (xMove || zMove) Camera::MoveLocal(xMove, 0.0f, zMove);

	Camera::Rotate(-pitch, yaw, 0);
	//Camera::LookAt(Vector3(0, 5, 0));

	MyUpdate();
}


void Start()
{
	Plane p;
	PointLight l;
	Cube c;

	DirectionalLight dl;
	dl.diffuse = { 0.5, 0.5, 0.5, 1 };
	dl.direction = { -0.5, -0.5, -0.5 };
	ObjectManager::Add(dl);

	p.scale = { 100, 100, 100 };
	c.position = { 0, 5, 0 };
	c.scale = { 0.5, 0.5, 0.5 };

	ObjectManager::Add(p);

	l.position = {-10, 5, 10};
	ObjectManager::Add(l);

	ObjectManager::Add(c);

	c.position = { 0, 0, 5 };
	ObjectManager::Add(c);



	//MyStart();
}



void Setting()
{
	Singlton.isGame = false;
	Singlton.cursorShow = true;
	Singlton.editor.color = { 70, 70, 70 };
	Singlton.foo.start = Start;
	Singlton.foo.update = Update;
	Singlton.scene.height = 800;
	Singlton.camera.rotationSmooth = 10;
	Singlton.camera.moveSmooth = 10;
}


