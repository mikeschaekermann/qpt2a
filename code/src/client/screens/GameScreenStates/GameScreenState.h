#include "../GameScreen.h"

#pragma once
class GameScreenState
{
public:
	GameScreenState(GameScreen * screen);
	virtual ~GameScreenState() {};

	virtual void update(float frameTime);
	virtual void draw3D();
	virtual void draw2D();

	virtual bool touchBegan(const TouchWay & touchWay);
	virtual void touchMoved(const TouchWay & touchWay);
	virtual bool mouseMove(MouseEvent event);
	virtual void touchEnded(const TouchWay & touchWay);
	virtual bool touchClick(TouchWay touchWay);
	virtual void resize(ResizeEvent event);
	virtual void onKeyInput(KeyEvent& e);
	virtual void mouseWheel(MouseEvent & e);

protected:
	void switchToState(GameScreenState * newState);
	
	GameScreen * screen;
};
