#pragma once

#include "Screen.h"
#include "../rendering/Cam.h"
#include "../../common/Config.h"
#include "../../common/Player.h"
#include "../../common/GameObjectContainer.h"
#include <unordered_map>
#include "../../common/network/messages/enum/CellType.h"
#include "boost/thread/mutex.hpp"

class CellClient;
class GameObjectClient;
class GameScreenState;
class GameScreenStateNeutral;
class GameScreenStateInMenu;
class GameScreenStateCreateCell;

class GameScreen :
	public Screen
{
public:	
	friend class GameScreenState;
	friend class GameScreenStateNeutral;
	friend class GameScreenStateInMenu;
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
	virtual bool touchClick(TouchWay touchWay);
	virtual void resize(ResizeEvent event);
	virtual void onKeyInput(KeyEvent& e);
	virtual void mouseWheel(MouseEvent & e);

	void addGameObjectToUpdate(GameObjectClient * gameObject);
	void addGameObjectToDraw(GameObjectClient * gameObject);
	void addGameObjectToCollide(GameObject * gameObject);
	void addCellToPick(CellClient * cell);

	void addIncompleteCell(CellClient * cell);
	void addIncompleteCell(
		unsigned int playerId, 
		CellType::Type type, 
		unsigned int cellId, 
		Vec3f position, 
		float angle
	);
	void completeCellById(unsigned int id);

	void switchToState(GameScreenState * newState);

private:
	vector<CellClient *> GameScreen::getCellsPicked(Vec2f position);
	
	/// current state of the screen
	GameScreenState * state;
	/// perspective cam for the game
	Cam cam;
	/// the world's radius, i.e. the radius of the Petri's dish
	float worldRadius;
	/// in-game menu for cell actions
	GUIItem * cellMenu;
	/// cell currently picked
	CellClient * pickedCell;

	/// mutex for container manipulation and reading
	boost::mutex containerMutex;
	
	/// all game objects registered to be updated
	GameObjectContainer<GameObject>			gameObjectsToUpdate;

	/// all game objects registered to be drawn
	GameObjectContainer<GameObjectClient>	gameObjectsToDraw;

	/// all game objects registered to collide with each other
	GameObjectContainer<GameObject>			gameObjectsToCollide;

	/// all cells registered to be pickable
	GameObjectContainer<CellClient>			cellsToPick;

	/// all incomplete cells
	GameObjectContainer<CellClient>			cellsIncomplete;
};