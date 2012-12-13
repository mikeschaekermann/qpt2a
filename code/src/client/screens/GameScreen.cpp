#include "GameScreen.h"
#include "../../common/network/NetworkManager.h"

GameScreen::GameScreen(ScreenManager& screenManager) :
	Screen(screenManager)
{
	
}

GameScreen::~GameScreen(void)
{
}

void GameScreen::update(float frameTime)
{
}

void GameScreen::draw()
{
	for (auto it = gameObjectsToDraw.begin(); it != gameObjectsToDraw.end(); ++it)
	{
		it->second->draw();
	}
}

void GameScreen::loadContent()
{

	/// if everything was loaded set the isInitialized to true
	isInitialized = true;
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
