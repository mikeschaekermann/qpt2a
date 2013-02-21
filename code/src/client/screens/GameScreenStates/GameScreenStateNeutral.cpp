#include "GameScreenStateNeutral.h"
#include "GameScreenStateInMenu.h"
#include "GameScreenStateSelectPolypeptides.h"
#include "../../managers/GameManager.h"

GameScreenStateNeutral::GameScreenStateNeutral(GameScreen * screen):
	GameScreenState(screen)
{
}

bool GameScreenStateNeutral::touchClick(TouchWay touchWay)
{
	return GameScreenState::touchClick(touchWay);
}

bool GameScreenStateNeutral::touchBegan(const TouchWay & touchWay)
{
	return GameScreenState::touchBegan(touchWay);
}

void GameScreenStateNeutral::touchMoved(const TouchWay & touchWay)
{
	return GameScreenState::touchMoved(touchWay);
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

	auto z = cam.getPosition().z;

	if((cam.getPosition().z > 200 && e.getWheelIncrement() > 0) || (cam.getPosition().z < 2700 && e.getWheelIncrement() < 0))
	{
		cam.setPosition(cam.getPosition() + ((cam.getFocus() - cam.getPosition()).normalized() * e.getWheelIncrement() * 100.f));
	}
}