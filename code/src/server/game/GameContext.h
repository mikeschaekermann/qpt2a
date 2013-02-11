#pragma once

#include "../../common/GameObject.h"
#include "../../common/GameObjectContainer.h"

#include <map>

class PlayerServer;

#define GAMECONTEXT GameContext::getInstance()

class GameContext
{
public:
	static GameContext * getInstance();

	GameObjectContainer<GameObject> & getActiveCells();
	GameObjectContainer<GameObject> & getInactiveCells();
	GameObjectContainer<GameObject> & getEnvironment();

	PlayerServer * getPlayer(unsigned id);
	void addPlayer(PlayerServer * player);
	std::map<unsigned, PlayerServer *> & getPlayerMap();

private:
	static GameContext *instance;

	GameContext();
	std::map<unsigned, PlayerServer *> playermap;
	GameObjectContainer<GameObject> activeCells;
	GameObjectContainer<GameObject> inactiveCells;
	GameObjectContainer<GameObject> environment;
};