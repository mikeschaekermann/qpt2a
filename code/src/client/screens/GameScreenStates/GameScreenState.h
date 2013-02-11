#include "../GameScreen.h"

#pragma once
class GameScreenState
{
public:
	GameScreenState(GameScreen * screen):
		screen(screen)
	{
	}

	virtual ~GameScreenState(void)
	{
	}

	virtual void update(float frameTime) {}
	virtual void draw3D() {}
	virtual void draw2D() {}

	virtual bool touchBegan(const TouchWay & touchWay) { return false; }
	virtual void touchMoved(const TouchWay & touchWay) {}
	virtual bool mouseMove(MouseEvent event) { return false; }
	virtual void touchEnded(TouchWay touchWay) {}
	virtual bool touchClick(TouchWay touchWay) { return false; }
	virtual void resize(ResizeEvent event) {}
	virtual void onKeyInput(KeyEvent& e) {}

protected:
	void switchToState(GameScreenState * newState) { screen->switchToState(newState); }
	
	GameScreen * screen;
};

