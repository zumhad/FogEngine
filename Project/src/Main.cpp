#include "Engine.h"
#include "Main.h"
void Setting()
{	Singlton.foo.start = MyStart;
	Singlton.isGame = true;
	Singlton.cursorShow = false;
	Singlton.game.color = { 1, 1, 1 };
}
#include "ApplicationEngine.h"
CREATE_APPLICATION(Setting)