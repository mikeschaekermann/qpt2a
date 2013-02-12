#include "GameScreenStateNeutral.h"
#include "GameScreenStateInMenu.h"
#include "../../managers/GameManager.h"

GameScreenStateNeutral::GameScreenStateNeutral(GameScreen * screen):
	GameScreenState(screen)
{
}

bool GameScreenStateNeutral::touchClick(TouchWay touchWay)
{
	auto cellsPicked = screen->getCellsPicked(touchWay.getCurrentPos());

	if (cellsPicked.size() > 0)
	{
		screen->switchToState(new GameScreenStateInMenu(screen, cellsPicked[0]));
		return true;
	}
}

void GameScreenStateNeutral::touchMoved(const TouchWay & touchWay)
{
	auto& cam = screen->cam;
	cam.setEyePoint(cam.getEyePoint() + Vec3f(-touchWay.getLastDeltaVector().x, touchWay.getLastDeltaVector().y, 0));
}

void GameScreenStateNeutral::onKeyInput(KeyEvent& e)
{
	auto& cam = screen->cam;

	if (e.getCode() == KeyEvent::KEY_ESCAPE)
	{
		SCREEN_MGR->openMenuScreen();
		GAME_MGR->releaseInstance();
	}
	else if(e.getCode() == KeyEvent::KEY_LEFT)
	{
		cam.setEyePoint(cam.getEyePoint() + Vec3f(10.f, 0.f, 0.f));
	}
	else if(e.getCode() == KeyEvent::KEY_RIGHT)
	{
		cam.setEyePoint(cam.getEyePoint() + Vec3f(-10.f, 0.f, 0.f));
	}
	else if(e.getCode() == KeyEvent::KEY_UP)
	{
		cam.setEyePoint(cam.getEyePoint() + Vec3f(0.f, -10.f, 0.f));
	}
	else if(e.getCode() == KeyEvent::KEY_DOWN)
	{
		cam.setEyePoint(cam.getEyePoint() + Vec3f(0.f, 10.f, 0.f));
	}
}

void GameScreenStateNeutral::mouseWheel(MouseEvent & e)
{
	auto& cam = screen->cam;
	cam.setEyePoint(cam.getEyePoint() + Vec3f(0.f, 0.f, -e.getWheelIncrement() * 100.f));
}