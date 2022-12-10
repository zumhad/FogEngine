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

	if (Input::Down(KEY_Q))
	{
		Cursor::SetVisible(!Cursor::GetVisible());
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
	float moveSpeed = 10.0f;
	float rotationSpeed = 150.0f;

	float rotate = (float)Input::Press(KEY_D) - (float)Input::Press(KEY_A);
	rotate *= rotationSpeed * Time::DeltaTime();

	float move = (float)Input::Press(KEY_W) - (float)Input::Press(KEY_S);
	move *= moveSpeed;

	Mesh& model = (Mesh&)ObjectManager::Get(1);

	static Vector3 vel = Vector3(0, 0, 0);
	static Vector3 currenVel = Vector3(0, 0, 0);
	currenVel = Vector3::SmoothDamp(currenVel, model.GetDirection() * move, vel, 0.1f);

	model.Move(currenVel * Time::DeltaTime());
	model.Rotate(Vector3(0, rotate, 0));

	static Vector3 vel1 = Vector3(0, 0, 0);
	static Vector3 currenVel1 = Vector3(0, 0, 0);
	currenVel1 = Vector3::SmoothDamp(currenVel1, model.GetDirection() * move, vel1, 0.2f);

	Camera::Move(currenVel1 * Time::DeltaTime());

	MyUpdate();
}

void foo2()
{
	Application::Exit();
}

void Start()
{
	Camera::SetRotationX(30);
	Camera::SetPosition(Vector3(0, 3, -5));

	Button b;
	b.action = foo2;
	b.width = 200;
	b.height = 50;
	b.x = 1000;
	b.y = 0;
	b.color = Color(1.0f, 0.0f, 0.0f);
	GUI::Add(b);

	DirectionalLight dl;
	//ObjectManager::Add(dl);

	PointLight l;
	l.color = Color(1.0f, 1.0f, 1.0f);
	l.position = Vector3(0.0f, 50.0f, 0.0f);
	l.power = 3.0f;
	l.range = 60;
	ObjectManager::Add(l);

	Mesh m1;
	m1.name = L"sphere.obj";
	m1.SetPosition(Vector3(0, 1, 0));
	m1.material.specular = { 1, 1, 1, 10 };
	m1.material.diffuse = { 1,0,0,1 };
	ObjectManager::Add(m1);


	Mesh m2;
	m2.name = L"plane.obj";
	m2.SetScale(Vector3(1000, 1, 1000));
	ObjectManager::Add(m2);

}



void Setting()
{
	Singlton.isGame = true;
	Singlton.cursorShow = false; 
	Singlton.scene.color = Color(0.7f, 0.7f, 0.7f);
	Singlton.editor.color = Color(1, 1, 1);
	Singlton.foo.start = Start;
	Singlton.foo.update = Update;
	//Singlton.fpsMax = 50;
	Singlton.camera.rotationSmooth = 500;
	Singlton.camera.moveSmooth = 1000;
}