#include "GameScreenState.h"
#include "GameScreenStateNeutral.h"

GameScreenState::GameScreenState(GameScreen * screen):
	screen(screen)
{
}

void GameScreenState::update(float frameTime) {}
void GameScreenState::draw3D() {}
void GameScreenState::draw2D() {}

bool GameScreenState::touchBegan(const TouchWay & touchWay) { return false; }
void GameScreenState::touchMoved(const TouchWay & touchWay) {}
bool GameScreenState::mouseMove(MouseEvent event) { return false; }
void GameScreenState::touchEnded(const TouchWay & touchWay) {}
bool GameScreenState::touchClick(TouchWay touchWay) { return false; }
void GameScreenState::resize(ResizeEvent event) {}
void GameScreenState::onKeyInput(KeyEvent& e)
{
	if (e.getCode() == KeyEvent::KEY_ESCAPE)
	{
		screen->switchToState(new GameScreenStateNeutral(screen));
	}
}
void GameScreenState::mouseWheel(MouseEvent & e) {}

void GameScreenState::switchToState(GameScreenState * newState) { screen->switchToState(newState); }