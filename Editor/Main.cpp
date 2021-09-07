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
	Edit::CameraSetRotationX(Clamp(Edit::CameraGetRotateX(), -XM_PIDIV2, XM_PIDIV2));
		
	if (Input::IsKeyDown(KEY_ESC))
		Edit::Exit();
}

void MyInit()
{
	
}

class Editor : public Application
{
public:
	Editor()
	{
		mProperties.sceneColor = RGB(255, 255, 255);
		mProperties.editorColor = RGB(70, 70, 70);
		mProperties.handleKey = MyHandler;
		mProperties.init = MyInit;
		//mProperties.captionHeight = 50;
		//mProperties.camera.position = { 0.0f, 3.0f, -10.0f };
		mProperties.camera.rotationSmooth = 5000.0f;
		mProperties.camera.moveSmooth = 2000.0f;
		//mProperties.maxFps = 0;
		//mProperties.sceneRect = { 0, 0, 1280, 720 };

#ifdef GAME_APP
		mProperties.isGame = true;
		mProperties.showCursor = true;
#else
		mProperties.isGame = false;
		mProperties.showCursor = true;
#endif
	}
};


CREATE_APPLICATION()