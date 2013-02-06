#include "ScreenManager.h"
#include "../screens/MenuScreen.h"
#include "../screens/GameScreen.h"

ScreenManager::ScreenManager(void):
	m_backgroundScreen(nullptr),
	menuScreen(new MenuScreen()),
	gameScreen(new GameScreen())
{
	this->currentScreen = menuScreen;
	m_backgroundScreen->loadContent();
	menuScreen->loadContent();
	gameScreen->loadContent();
}

ScreenManager::~ScreenManager(void)
{
	delete menuScreen;
	delete gameScreen;
	delete currentScreen;
}

void ScreenManager::update(float frameTime)
{
	currentScreen->update(frameTime);
	
	if(m_backgroundScreen)
	{
		m_backgroundScreen->update(frameTime);
	}
}

void ScreenManager::draw()
{
	currentScreen->draw();

	if(m_backgroundScreen)
	{
		m_backgroundScreen->draw();
	}
}

void ScreenManager::openScreen()
{
	currentScreen = m_backgroundScreen;
}

void ScreenManager::openMenuScreen()
{
	currentScreen = menuScreen;
}

void ScreenManager::openGameScreen()
{
	currentScreen = gameScreen;
}

GameScreen & ScreenManager::getGameScreen() const
{
	return *gameScreen;
}

void ScreenManager::fadeToBlack(float alpha)
{
	auto windowSize = getWindowSize();
	color(cinder::ColorA(0, 0, 0, alpha));
	drawSolidRect(cinder::Rectf(0.0f, 0.0f, (float)windowSize.x, (float)windowSize.y));
}

void ScreenManager::touchBegan(const TouchWay & touchWay)
{
	currentScreen->touchBegan(touchWay);
}

void ScreenManager::touchMoved(const TouchWay & touchWay)
{
	currentScreen->touchMoved(touchWay);
}

void ScreenManager::mouseMove( MouseEvent event )
{
	currentScreen->mouseMove(event);
}

void ScreenManager::touchEnded(TouchWay touchWay)
{
	currentScreen->touchEnded(touchWay);

	if (touchWay.isClick())
	{
		currentScreen->touchClick(touchWay);
	}
}

void ScreenManager::resize(ResizeEvent event)
{
	currentScreen->resize(event);
}

ScreenManager * ScreenManager::getInstance()
{
	return instance;
}

ScreenManager * ScreenManager::instance = new ScreenManager();