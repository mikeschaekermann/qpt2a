#pragma once

#include "Screen.h"
#include "../rendering/Cam.h"
#include "../../common/Config.h"
#include "../actors/GameObjectClient.h"
#include "../../common/Player.h"
#include "../../common/GameObjectContainer.h"
#include <unordered_map>

class CellClient;

class GameScreen :
	public Screen
{
public:
	typedef unordered_map<unsigned, GameObjectClient*> IdGameObjectClientMap;
	
	GameScreen();
	virtual ~GameScreen(void);

	virtual void update(float frameTime);
	virtual void draw();
	void setWorldRadius(float radius) { worldRadius = radius; }
	void zoomToWorld();


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
	void addCellToPick(CellClient * cell, bool collidable);

private:
	void pickCell(GameObject * cell);
	void unpickCell();
	
	/// perspective cam for the game
	Cam cam;
	/// the world's radius, i.e. the radius of the Petri's dish
	float worldRadius;
	/// in-game menu for cell actions
	GUIItem * cellMenu;
	/// currently picked cell
	GameObject * pickedCell;

	/// all game objects registered to be updated
	GameObjectContainer<GameObject>			gameObjectsToUpdate;

	/// all game objects registered to be drawn
	GameObjectContainer<GameObjectClient>	gameObjectsToDraw;

	/// all game objects registered to collide with each other
	GameObjectContainer<GameObject>			gameObjectsToCollide;

	/// all game objects registered to be pickable
	GameObjectContainer<CellClient>			cellsToPick;
};