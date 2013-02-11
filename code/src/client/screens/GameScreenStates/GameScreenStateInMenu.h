#pragma once

#include "GameScreenState.h"

class CellClient;

class GameScreenStateInMenu:
	public GameScreenState
{
public:

	GameScreenStateInMenu(GameScreen* screen, CellClient * pickedCell);
	virtual ~GameScreenStateInMenu(void);
	
	virtual bool touchClick(TouchWay touchWay);

protected:
	CellClient * pickedCell;
};
