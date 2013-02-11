#pragma once

#include "Screen.h"
#include "../rendering/Cam.h"
#include "../../common/Config.h"
#include "../actors/GameObjectClient.h"
#include "../../common/Player.h"

#include "cinder/Vector.h"
#include "cinder/gl/Texture.h"

class MenuScreen :
	public Screen
{
public:
	MenuScreen();
	virtual ~MenuScreen(void);

	void terminateServer();

	STARTUPINFO serverStartupInfo;
	PROCESS_INFORMATION serverProcessInfo;
};