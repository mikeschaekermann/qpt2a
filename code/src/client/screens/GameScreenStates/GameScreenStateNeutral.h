#pragma once
#include "GameScreenState.h"

class GameScreenStateNeutral :
	public GameScreenState
{
public:
	GameScreenStateNeutral(GameScreen * screen);

	virtual bool touchClick(TouchWay touchWay);
	virtual void touchMoved(const TouchWay & touchWay);
	virtual void onKeyInput(KeyEvent& e);
	virtual void mouseWheel(MouseEvent & e);
};
