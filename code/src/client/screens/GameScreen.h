#pragma once

#include "Screen.h"
#include "../rendering/Cam.h"
#include "../../common/Config.h"
#include "../../common/Player.h"
#include "../../common/GameObjectContainer.h"
#include <unordered_map>

class CellClient;
class GameObjectClient;
class GameScreenState;
class GameScreenStateNeutral;
class GameScreenStateCreateCell;

class GameScreen :
	public Screen
{
public:	
	friend class GameScreenState;
	friend class GameScreenStateNeutral;
	friend class GameScreenStateCreateCell;

	GameScreen();
	virtual ~GameScreen(void);

	virtual void update(float frameTime);
	virtual void draw();
	void setWorldRadius(float radius) { worldRadius = radius; }
	void zoomToWorld();

	virtual bool touchBegan(const TouchWay & touchWay);
	virtual void touchMoved(const TouchWay & touchWay);
	virtual bool mouseMove(MouseEvent event);
	virtual void touchEnded(TouchWay touchWay);
	virtual void touchClick(TouchWay touchWay);
	virtual void resize(ResizeEvent event);
	virtual void onKeyInput(KeyEvent& e);
	virtual void mouseWheel(MouseEvent & e);

	void addGameObjectToUpdate(GameObjectClient * gameObject, bool collidable);
	void addGameObjectToDraw(GameObjectClient * gameObject, bool collidable);
	void addCellToPick(CellClient * cell, bool collidable);

	void switchToState(GameScreenState * newState);

private:
	void pickCell(CellClient * cell);
	void unpickCell();
	
	/// current state of the screen
	GameScreenState * state;
	/// perspective cam for the game
	Cam cam;
	/// the world's radius, i.e. the radius of the Petri's dish
	float worldRadius;
	/// in-game menu for cell actions
	GUIItem * cellMenu;
	/// currently picked cell
	CellClient * pickedCell;

	/// all game objects registered to be updated
	GameObjectContainer<GameObject>			gameObjectsToUpdate;

	/// all game objects registered to be drawn
	GameObjectContainer<GameObjectClient>	gameObjectsToDraw;

	/// all game objects registered to collide with each other
	GameObjectContainer<GameObject>			gameObjectsToCollide;

	/// all game objects registered to be pickable
	GameObjectContainer<CellClient>			cellsToPick;
};