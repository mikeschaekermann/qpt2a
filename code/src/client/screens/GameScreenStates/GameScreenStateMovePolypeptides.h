#pragma once
#include "GameScreenState.h"
#include "GameScreenStateNeutral.h"

class GameScreenStateMovePolypeptides :
	public GameScreenState
{
public:
	GameScreenStateMovePolypeptides(GameScreen * screen);

	virtual bool touchClick(TouchWay touchWay);
	virtual void touchMoved(const TouchWay & touchWay);

private:
	GameScreenStateNeutral neutralState;
};
