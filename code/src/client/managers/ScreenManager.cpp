#include "ScreenManager.h"
#include "../screens/MenuScreen.h"
#include "../screens/GameScreen.h"
#include "../screens/ConnectScreen.h"
#include "../screens/CreditsScreen.h"

ScreenManager::ScreenManager(void):
	m_backgroundScreen(nullptr),
	menuScreen(new MenuScreen()),
	gameScreen(new GameScreen()),
	connectScreen(new ConnectScreen()),
	creditsScreen(new CreditsScreen())
{
	openMenuScreen();
}

ScreenManager::~ScreenManager(void)
{
	if (menuScreen != nullptr)
	{
		menuScreen->terminateServer();
	}
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

void ScreenManager::openConnectScreen()
{
	currentScreen = connectScreen;
}

void ScreenManager::openCreditsScreen()
{
	currentScreen = creditsScreen;
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
	if(!instance)
	{
		instance = new ScreenManager();
	}

	return instance;
}

void ScreenManager::onKeyInput(KeyEvent& e) const
{
	currentScreen->onKeyInput(e);
}

ScreenManager * ScreenManager::instance = nullptr;