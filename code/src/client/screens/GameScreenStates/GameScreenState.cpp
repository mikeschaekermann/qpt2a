#include "GameScreenState.h"
#include "GameScreenStateNeutral.h"
#include "GameScreenStateInMenu.h"
#include "GameScreenStateSelectPolypeptides.h"

GameScreenState::GameScreenState(GameScreen * screen):
	screen(screen)
{
}

void GameScreenState::update(float frameTime) {}
void GameScreenState::draw3D() {}
void GameScreenState::draw2D() {}

bool GameScreenState::touchBegan(const TouchWay & touchWay) 
{
	auto cellsPicked = screen->getCellsPicked(touchWay.getCurrentPos());

	if (cellsPicked.size() > 0 && cellsPicked[0]->getPolypeptides().size())
	{
		screen->switchToState(new GameScreenStateSelectPolypeptides(screen, cellsPicked[0]));
	}
	return true;
}

void GameScreenState::touchMoved(const TouchWay & touchWay) 
{
	auto& cam = RenderManager::getInstance()->cam;
	
	auto curPos2D = touchWay.getCurrentPos();
	auto prevPos2D = curPos2D - touchWay.getLastDeltaVector();

	auto curPos3D = cam.screenToWorldPlane(curPos2D);
	auto prevPos3D = cam.screenToWorldPlane(prevPos2D);

	auto shift = prevPos3D - curPos3D;

	cam.setPosition(cam.getPosition() + shift);
}

bool GameScreenState::mouseMove(MouseEvent event) { return false; }
void GameScreenState::touchEnded(const TouchWay & touchWay) {}

bool GameScreenState::touchClick(TouchWay touchWay) 
{ 
	if (touchWay.getTrigger() == TouchWay::LEFT)
	{
		auto cellsPicked = screen->getCellsPicked(touchWay.getCurrentPos());

		if (cellsPicked.size() == 0)
		{
			screen->switchToState(new GameScreenStateNeutral(screen));
			return false;
		}
		else
		{
			screen->switchToState(new GameScreenStateInMenu(screen, cellsPicked[0]));
		}
	}
	else
	{
		screen->switchToState(new GameScreenStateNeutral(screen));
	}

	return false;
}

void GameScreenState::resize(ResizeEvent event) {}
void GameScreenState::onKeyInput(KeyEvent& e)
{
	if (e.getCode() == KeyEvent::KEY_ESCAPE || e.getChar() == 'f')
	{
		screen->switchToState(new GameScreenStateNeutral(screen));
	}
}
void GameScreenState::mouseWheel(MouseEvent & e) {}

void GameScreenState::switchToState(GameScreenState * newState) { screen->switchToState(newState); }