#include "ScreenManager.h"
#include "../screens/MenuScreen.h"
#include "../screens/GameScreen.h"

ScreenManager::ScreenManager(void):
	m_backgroundScreen(nullptr)
{
}

ScreenManager::~ScreenManager(void)
{
	while (!m_screenStack.empty())
	{
		auto tmp = m_screenStack.top();
		m_screenStack.pop();
		delete tmp;
	}
}

void ScreenManager::update(float frameTime)
{
	m_screenStack.top()->update(frameTime);
	
	if(m_backgroundScreen)
	{
		m_backgroundScreen->update(frameTime);
	}
}

void ScreenManager::draw()
{
	m_screenStack.top()->draw();

	if(m_backgroundScreen)
	{
		m_backgroundScreen->draw();
	}
}

void ScreenManager::openScreen(Screen* screen)
{
	screen->loadContent();

	m_screenStack.push(screen);
}

void ScreenManager::openMenuScreen(MenuScreen * menuScreen)
{
	this->menuScreen = menuScreen;

	openScreen(menuScreen);
}

void ScreenManager::openGameScreen(GameScreen* gameScreen)
{
	this->gameScreen = gameScreen;

	openScreen(gameScreen);
}

GameScreen & ScreenManager::getGameScreen() const
{
	return *gameScreen;
}

void ScreenManager::closeScreen()
{
	m_screenStack.pop();
}

void ScreenManager::fadeToBlack(float alpha)
{
	auto windowSize = getWindowSize();
	color(cinder::ColorA(0, 0, 0, alpha));
	drawSolidRect(cinder::Rectf(0.0f, 0.0f, (float)windowSize.x, (float)windowSize.y));
}

void ScreenManager::touchBegan(const TouchWay & touchWay)
{
	m_screenStack.top()->touchBegan(touchWay);
}

void ScreenManager::touchMoved(const TouchWay & touchWay)
{
	m_screenStack.top()->touchMoved(touchWay);
}

void ScreenManager::touchEnded(TouchWay touchWay)
{
	m_screenStack.top()->touchEnded(touchWay);

	if (touchWay.isClick())
	{
		m_screenStack.top()->touchClick(touchWay);
	}
}

void ScreenManager::resize(ResizeEvent event)
{
	m_screenStack.top()->resize(event);
}

ScreenManager * ScreenManager::getInstance()
{
	return instance;
}

ScreenManager * ScreenManager::instance = new ScreenManager();