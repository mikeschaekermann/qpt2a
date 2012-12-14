#include "GameScreen.h"
#include "../../common/network/NetworkManager.h"

GameScreen::GameScreen(ScreenManager& screenManager) :
	Screen(screenManager)
{
	auto screenSize = getWindowSize();
	cam = Cam(screenSize.x, screenSize.y, toRadians(45.f), 1.f, 10.f);
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

	auto pointInWorld = cam.screenToWorld(touchWay.getCurrentPos());

	LOG_INFO(concatenate(concatenate("Point on Screen: ", touchWay.getCurrentPos().x), touchWay.getCurrentPos().y));
	LOG_INFO(concatenate(concatenate(concatenate("Point in 3D: ", pointInWorld.x), pointInWorld.y), pointInWorld.z));
};
