#include "GameScreen.h"
#include "../../common/network/NetworkManager.h"

GameScreen::GameScreen()
{
	auto screenSize = getWindowSize();
	cam = Cam(screenSize.x, screenSize.y, CONFIG_FLOAT1("data.rendering.camera.fieldOfView"), CONFIG_FLOAT1("data.rendering.camera.nearPlane"), CONFIG_FLOAT1("data.rendering.camera.farPlane"));

	cam
		.setPosition(Vec3f( 0.0f, 0.0f, 0.0f ))
		.setFocus(Vec3f::zero());
}

GameScreen::~GameScreen(void)
{
}

void GameScreen::update(float frameTime)
{
}

void GameScreen::draw()
{
	gl::enableDepthWrite();
	gl::enableDepthRead();

	gl::setMatrices(cam);

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

	auto pointInWorldPlane = cam.screenToWorldPlane(touchWay.getCurrentPos());

	auto objectsPicked = gameObjectsToPick.pick(pointInWorldPlane);

	if (objectsPicked.size() > 0)
	{
		LOG_INFO("number of objects picked:");
		LOG_INFO(objectsPicked.size());
	}
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

void GameScreen::resize(ResizeEvent event)
{
	cam.setAspectRatio(getWindowAspectRatio());
}

void GameScreen::addGameObjectToUpdate(GameObjectClient * gameObject, bool collidable)
{
	if (collidable)
	{
		gameObjectsToCollide.createGameObject(gameObject);
	}

	gameObjectsToUpdate.createGameObject(gameObject);
}

void GameScreen::addGameObjectToDraw(GameObjectClient * gameObject, bool collidable)
{
	addGameObjectToUpdate(gameObject, collidable);

	gameObjectsToDraw.insert(make_pair(gameObject->getId(), gameObject));
}

void GameScreen::addGameObjectToPick(GameObjectClient * gameObject, bool collidable)
{
	addGameObjectToDraw(gameObject, collidable);

	gameObjectsToPick.createGameObject(gameObject);
}

void GameScreen::zoomToWorld()
{
	/// world radius = sin(camera's fov / 2) * camera's distance
	/// camera's distance = world radius / sin(camera's fov / 2)

	float camDistance = worldRadius / sin(cam.getFov() / 2 / 180.0 * M_PI);

	cam
		.setPosition(Vec3f(0, 0, camDistance))
		.setFocus(Vec3f::zero());
}