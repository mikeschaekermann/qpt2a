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
	return false;
}

void GameScreenStateNeutral::touchMoved(const TouchWay & touchWay)
{
	auto& cam = RenderManager::getInstance()->cam;
	
	auto curPos2D = touchWay.getCurrentPos();
	auto prevPos2D = curPos2D - touchWay.getLastDeltaVector();

	auto curPos3D = cam.screenToWorldPlane(curPos2D);
	auto prevPos3D = cam.screenToWorldPlane(prevPos2D);

	auto shift = prevPos3D - curPos3D;

	cam.setPosition(cam.getPosition() + shift);
}

void GameScreenStateNeutral::onKeyInput(KeyEvent& e)
{
	auto& cam = RenderManager::getInstance()->cam;

	if (e.getCode() == KeyEvent::KEY_ESCAPE)
	{
		SCREEN_MGR->openMenuScreen();
		GAME_MGR->releaseInstance();
		SOUND_PLAYER->stopAllSounds();
		setFullScreen(false);
	}
	else if(e.getCode() == KeyEvent::KEY_LEFT)
	{
		cam.setPosition(cam.getPosition() + Vec3f(10.f, 0.f, 0.f));
	}
	else if(e.getCode() == KeyEvent::KEY_RIGHT)
	{
		cam.setPosition(cam.getPosition() + Vec3f(-10.f, 0.f, 0.f));
	}
	else if(e.getCode() == KeyEvent::KEY_UP)
	{
		cam.setPosition(cam.getPosition() + Vec3f(0.f, -10.f, 0.f));
	}
	else if(e.getCode() == KeyEvent::KEY_DOWN)
	{
		cam.setPosition(cam.getPosition() + Vec3f(0.f, 10.f, 0.f));
	}
}

void GameScreenStateNeutral::mouseWheel(MouseEvent & e)
{
	auto& cam = RenderManager::getInstance()->cam;
	cam.setPosition(cam.getPosition() + Vec3f(0.f, 0.f, -e.getWheelIncrement() * 100.f));
}