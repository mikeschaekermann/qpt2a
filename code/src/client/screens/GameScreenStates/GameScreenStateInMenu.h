#pragma once

#include "GameScreenState.h"

class CellClient;

class GameScreenStateInMenu:
	public GameScreenState
{
public:

	GameScreenStateInMenu(GameScreen* screen, CellClient * pickedCell);
	virtual ~GameScreenStateInMenu(void);
	
	virtual bool touchBegan(const TouchWay & touchWay);
	virtual bool touchClick(TouchWay touchWay);
	virtual bool mouseMove(MouseEvent event);

protected:
	CellClient * pickedCell;
	bool isStemcell;
};
