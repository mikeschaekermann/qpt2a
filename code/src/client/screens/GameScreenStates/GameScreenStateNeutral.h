#pragma once
#include "GameScreenState.h"

class GameScreenStateNeutral :
	public GameScreenState
{
public:
	GameScreenStateNeutral(GameScreen * screen):
		GameScreenState(screen)
	{
	}

	virtual bool touchBegan(const TouchWay & touchWay)
	{
		auto pointInWorldPlane = screen->cam.screenToWorldPlane(touchWay.getCurrentPos());
		auto cellsPicked = screen->cellsToPick.pick(pointInWorldPlane);

		if (cellsPicked.size() > 0)
		{
			screen->pickCell(cellsPicked[0]);
			return true;
		}
		else
		{
			screen->unpickCell();
			return false;
		}
	}

	virtual void onKeyInput(KeyEvent& e)
	{
		if (e.getCode() == KeyEvent::KEY_ESCAPE)
		{
			screen->cellMenu->setVisible(false);
		}
	}
};

