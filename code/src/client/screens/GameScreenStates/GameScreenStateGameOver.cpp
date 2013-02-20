#include "GameScreenStateGameOver.h"
#include "GameScreenStateInMenu.h"
#include "../../managers/GameManager.h"

GameScreenStateGameOver::GameScreenStateGameOver(GameScreen * screen):
	GameScreenState(screen),
	layerOpacity(0),
	maxLayerOpacity(CONFIG_FLOAT("data.ingamefeedback.gameOver.maxLayerOpacity")),
	layerOpacityChangeSpeed(CONFIG_FLOAT("data.ingamefeedback.gameOver.layerOpacityChangeSpeed"))
{
}

void GameScreenStateGameOver::update(float frameTime)
{
	layerOpacity += layerOpacityChangeSpeed * frameTime;

	if (layerOpacity > maxLayerOpacity)
	{
		layerOpacity = maxLayerOpacity;
	}
}

void GameScreenStateGameOver::draw2D()
{
	gl::color(ColorA(0, 0, 0, layerOpacity));
	gl::drawSolidRect(Rectf(Vec2f(0, 0), getWindowSize()));
}

bool GameScreenStateGameOver::touchClick(TouchWay touchWay)
{
	return false;
}

void GameScreenStateGameOver::touchMoved(const TouchWay & touchWay)
{
	auto& cam = RenderManager::getInstance()->cam;
	cam.setEyePoint(cam.getEyePoint() + Vec3f(-touchWay.getLastDeltaVector().x, touchWay.getLastDeltaVector().y, 0));
}

void GameScreenStateGameOver::onKeyInput(KeyEvent& e)
{
	auto& cam = RenderManager::getInstance()->cam;

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

void GameScreenStateGameOver::mouseWheel(MouseEvent & e)
{
	auto& cam = RenderManager::getInstance()->cam;
	cam.setEyePoint(cam.getEyePoint() + Vec3f(0.f, 0.f, -e.getWheelIncrement() * 100.f));
}