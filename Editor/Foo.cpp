#include "Foo.h"
#include "Main.h"



class Vector3
{
public:
	Vector3(float x = 0.0f, float y = 0.0f, float z = 0.0f)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	operator DirectX::XMFLOAT3()
	{
		return DirectX::XMFLOAT3(x, y, z);
	}

	operator DirectX::XMVECTOR()
	{
		return DirectX::XMVectorSet(x, y, z, 1);
	}

public:
	float x, y, z;
};

class Vector4
{
public:
	Vector4(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	operator DirectX::XMFLOAT4()
	{
		return DirectX::XMFLOAT4(x, y, z, w);
	}

	operator DirectX::XMVECTOR()
	{
		return DirectX::XMVectorSet(x, y, z, w);
	}

public:
	float x, y, z, w;
};


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
	cube.position = Vector3((rand() % 10 - 5) * 2, -1, (rand() % 10 - 5) * 2);
	ObjectManager::Add(cube);
}

void foo2()
{
	Application::Exit();
}

void Start()
{
	Camera::SetPosition(Vector3(0, 5, 0));

	Button b;
	b.width = 200;
	b.height = 50;
	b.x = 1000;
	b.y = 200;
	b.color = { 1,0,0 };
	GUI::Add(b);

	PointLight l;
	l.diffuse = { 0.5, 0.2, 0.1, 1 };
	l.position = { 0, 20, 10 };
	l.att = { 0, 0.1, 0 };
	l.specular = { 1,1,1, 50 };
	ObjectManager::Add(l);

	Model m;
	m.name = L"FinalBaseMesh.obj";
	m.material.specular = { 1, 1, 1, 50 };
	ObjectManager::Add(m);

	Plane p;
	p.scale = { 10, 1, 10 };
	ObjectManager::Add(p);
}



void Setting()
{
	Singlton.isGame = false;
	Singlton.cursorShow = true;
	Singlton.scene.color = { 0.7, 0.7, 0.7 };
	Singlton.editor.color = { 1, 1, 1 };
	Singlton.foo.start = Start;
	Singlton.foo.update = Update;
	/*Singlton.game.width = 800;
	Singlton.game.height = 600;
	Singlton.resolution.width = 800;
	Singlton.resolution.height = 600;
	Singlton.fpsMax = 0;
	Singlton.foo.update = Update;
	Singlton.scene.height = 800;
	Singlton.camera.rotationSmooth = 500;
	Singlton.camera.moveSmooth = 1000;*/
}