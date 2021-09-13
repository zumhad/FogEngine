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

void MyHandler()
{
	float dt = Time::DeltaTime();
	float mouseSpeed = 0.001f * 2.0f;
	float yaw = Input::GetMouseAxis(MOUSE_X) * mouseSpeed;
	float pitch = Input::GetMouseAxis(MOUSE_Y) * mouseSpeed;

	float zMove = Input::IsKeyPress(KEY_W) * 1.0f - Input::IsKeyPress(KEY_S) * 1.0f; zMove *= 10;
	float xMove = Input::IsKeyPress(KEY_D) * 1.0f - Input::IsKeyPress(KEY_A) * 1.0f; xMove *= 10;
	if (xMove || zMove) Edit::CameraMoveLocal(xMove, 0.0f, zMove);

	Edit::CameraRotate(-pitch, yaw, 0);
	//Edit::CameraSetRotationX(Clamp(Edit::CameraGetRotateX(), -XM_PIDIV2, XM_PIDIV2));
		
	if (Input::IsKeyDown(KEY_ESC))
		Edit::Exit();
}

void MyInit()
{
	/*Cube c;
	ObjectManager::Add(c);*/
}

class Editor : public Application
{
public:
	Editor()
	{
		prop.camera.moveSmooth = 20;
		prop.camera.rotationSmooth = 50;
		prop.foo.update = MyHandler;
		prop.editor.color = RGB(70, 70, 70);
		//prop.fpsMax = 300;
		//prop.isGame = true;
		//prop.cursorShow = false;
	}
};


CREATE_APPLICATION()