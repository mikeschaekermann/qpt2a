#pragma once
#include "GameScreenState.h"
#include "GameScreenStateNeutral.h"

class GameScreenStateMovePolypeptides :
	public GameScreenState
{
public:
	GameScreenStateMovePolypeptides(GameScreen * screen);

	virtual void draw2D();

	virtual bool touchClick(TouchWay touchWay);
	virtual bool touchBegan(const TouchWay & touchWay);
	virtual void touchMoved(const TouchWay & touchWay);
	virtual bool mouseMove(MouseEvent event);

private:
	GameScreenStateNeutral neutralState;
	ci::Vec2f position;
};
