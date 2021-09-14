#include "Engine.h"

#ifdef GAME_APP
	#undef GAME_APP
#endif



//active
#ifdef GAME_APP
	///  GAME  ///
#else
	/// ENGINE ///
#endif


void MyUpdate()
{
	float mouseSpeed = 0.001f * 2.0f;
	float yaw = Input::GetMouseAxis(MOUSE_X) * mouseSpeed;
	float pitch = Input::GetMouseAxis(MOUSE_Y) * mouseSpeed;

	float zMove = Input::IsKeyPress(KEY_W) * 1.0f - Input::IsKeyPress(KEY_S) * 1.0f; zMove *= 10;
	float xMove = Input::IsKeyPress(KEY_D) * 1.0f - Input::IsKeyPress(KEY_A) * 1.0f; xMove *= 10;
	if (xMove || zMove) Edit::CameraMoveLocal(xMove, 0.0f, zMove);

	Edit::CameraRotate(-pitch, yaw, 0);

	static bool test = false;
	if (Input::IsMouseDown(MOUSE_LEFT) && !Edit::CursorInScene())
		test = true;
	if(Input::IsMouseUp(MOUSE_LEFT))
		test = false;

	if (Input::IsMousePress(MOUSE_LEFT) && test)
	{
		Edit::SetSceneX(Input::GetCursorX());
		Edit::SetSceneY(Input::GetCursorY());
	}


	if (Input::IsKeyDown(KEY_ESC))
		Edit::Exit();
}


void MyStart()
{
	Cube cube;


	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			cube.mPosition = Vector3(j*3, 0, i * 3);
			ObjectManager::Add(cube);
		}
	}
}

class Editor : public Application
{
public:
	Editor()
	{
		prop.camera.moveSmooth = 20;
		prop.camera.rotationSmooth = 50000;
		prop.foo.update = MyUpdate;
		prop.foo.start = MyStart;
		prop.editor.color = RGB(70, 70, 70);
		prop.fpsMax = 0;
		prop.isGame = true;
		prop.cursorShow = false;
	}
};


CREATE_APPLICATION()