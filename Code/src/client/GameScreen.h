#pragma once
#include "Screen.h"

class GameScreen :
	public Screen
{
public:
	GameScreen(ScreenManager& screenManager);
	virtual ~GameScreen(void);
};

