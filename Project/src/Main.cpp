#include "Engine.h"
#include "Main.h"
void Setting()
{	Singlton.foo.start = MyStart;
	Singlton.isGame = true;
	Singlton.cursorShow = false;
}
#include "ApplicationEngine.h"
CREATE_APPLICATION(Setting)