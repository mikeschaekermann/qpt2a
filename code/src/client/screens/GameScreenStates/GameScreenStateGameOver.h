#pragma once
#include "GameScreenState.h"

class GameScreenStateGameOver :
	public GameScreenState
{
public:
	GameScreenStateGameOver(GameScreen * screen);

	virtual void draw2D();
	virtual void update(float frameTime);

	virtual bool touchClick(TouchWay touchWay);
	virtual void touchMoved(const TouchWay & touchWay);
	virtual void onKeyInput(KeyEvent& e);
	virtual void mouseWheel(MouseEvent & e);

private:
	float layerOpacity, maxLayerOpacity, layerOpacityChangeSpeed;
};
