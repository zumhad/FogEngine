#pragma once

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



void MyUpdate();
void Update();
void Start();
void Setting();