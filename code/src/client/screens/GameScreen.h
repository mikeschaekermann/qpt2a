#pragma once

#include "Screen.h"
#include "../rendering/Cam.h"
#include "../../common/Config.h"
#include "../actors/GameObjectClient.h"
#include "../../common/Player.h"
#include "../../common/GameObjectContainer.h"

class GameScreen :
	public Screen
{
public:
	typedef map<unsigned int, GameObjectClient*> IdGameObjectClientMap;
	
	GameScreen(ScreenManager& screenManager);
	virtual ~GameScreen(void);

	virtual void update(float frameTime);
	virtual void draw();
	void setWorldRadius(float radius) { worldRadius = radius; }
	void zoomToWorld();

	virtual void loadContent();

	/**
		@brief event method called when a new touch has begun
		@param touchWay			touch way of the newly begun touch
	 */
	virtual void touchBegan(const TouchWay & touchWay);
	/**
		@brief event method called when an existing touch has moved
		@param touchWay			touch way of the touch, concerned
	 */
	virtual void touchMoved(const TouchWay & touchWay);
	/**
		@brief event method called when an existing touch has ended
		@param touchWay			touch way of the touch, concerned; must be called by copy,
								because touch way object will be destroyed after call to this method
	 */
	virtual void touchEnded(TouchWay touchWay);
	/**
		@brief event method called when an touch was released as a click (i.e. no drag!)
		@param touchWay			touch way of the touch, concerned; must be called by copy,
								because touch way object will be destroyed after call to this method
	 */
	virtual void touchClick(TouchWay touchWay);

	virtual void resize(ResizeEvent event);

	void addGameObjectToUpdate(GameObjectClient * gameObject, bool collidable);
	void addGameObjectToDraw(GameObjectClient * gameObject, bool collidable);
	void addGameObjectToPick(GameObjectClient * gameObject, bool collidable);

private:
	/// perspective cam for the game
	Cam cam;
	/// the world's radius, i.e. the radius of the Petri's dish
	float worldRadius;

	/// all game objects registered to be updated
	GameObjectContainer		gameObjectsToUpdate;

	/// all game objects registered to be drawn
	IdGameObjectClientMap	gameObjectsToDraw;

	/// all game objects registered to collide with each other
	GameObjectContainer		gameObjectsToCollide;

	/// all game objects registered to be pickable
	GameObjectContainer		gameObjectsToPick;
};