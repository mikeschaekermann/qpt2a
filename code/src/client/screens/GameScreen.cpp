#include "GameScreen.h"
#include "../../common/network/NetworkManager.h"

GameScreen::GameScreen(ScreenManager& screenManager) :
	Screen(screenManager),
	m_pScene(nullptr)
{
	
}

GameScreen::~GameScreen(void)
{
}

void GameScreen::update(float frameTime)
{
	if(m_pScene != nullptr)
	{
		m_pScene->update(frameTime);
	}
}

void GameScreen::draw()
{
	if(m_pScene != nullptr)
	{
		m_pScene->draw();
	}
}

void GameScreen::loadContent()
{

	/// if everything was loaded set the isInitialized to true
	m_bIsInitialized = true;
}

void GameScreen::touchBegan(const TouchWay & touchWay)
{
	LOG_INFO("touch way started");
};

void GameScreen::touchMoved(const TouchWay & touchWay)
{
	LOG_INFO("touch way moved");
};

void GameScreen::touchEnded(TouchWay touchWay)
{
	LOG_INFO("touch way ended");
};

void GameScreen::touchClick(TouchWay touchWay)
{
	LOG_INFO("touch click!");
};
