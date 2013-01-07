#include "GameScreen.h"
#include "../../common/network/NetworkManager.h"

GameScreen::GameScreen(ScreenManager& screenManager) :
	Screen(screenManager)
{
	auto screenSize = getWindowSize();
	cam = Cam(screenSize.x, screenSize.y, toRadians(1.f), 0.1f, 1000.f);
	
	CameraPersp mCam;
	mCam.setPerspective(60.0f, getWindowAspectRatio(), 5.0f, 3000.0f);

	auto eye	= Vec3f( 0.0f, 0.0f, 50.0f );
	auto center	= Vec3f::zero();
	auto up		= Vec3f::yAxis();
	
	mCam.lookAt(eye, center, up);
	gl::setMatrices(mCam);
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

void GameScreen::addGameObjectToUpdate(GameObjectClient * gameObject, bool collidable)
{
	/*if (collidable)
	{
		gameObjectContainer.createGameObject(gameObject);
	}*/

	gameObjectsToUpdate.insert(make_pair(gameObject->getId(), gameObject));
}

void GameScreen::addGameObjectToDraw(GameObjectClient * gameObject, bool collidable)
{
	addGameObjectToUpdate(gameObject, collidable);

	gameObjectsToDraw.insert(make_pair(gameObject->getId(), gameObject));
}

void GameScreen::addGameObjectToPick(GameObjectClient * gameObject, bool collidable)
{
	addGameObjectToDraw(gameObject, collidable);

	gameObjectsToPick.insert(make_pair(gameObject->getId(), gameObject));
}