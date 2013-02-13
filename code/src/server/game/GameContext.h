#pragma once

#include <boost\thread\mutex.hpp>

#include "../../common/GameObject.h"
#include "../../common/GameObjectContainer.h"

#include <map>

class PlayerServer;
class NetworkManager;
class Game;

#define GAMECONTEXT GameContext::getInstance()
#define NETWORKMANAGER GameContext::getInstance()->getNetworkManager()

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

	NetworkManager * getNetworkManager();
	void initializeNetworkManager(int port, Game * game);

	boost::mutex & getMutex();

private:
	// Singleton stuff
	static GameContext *instance;
	GameContext();

	NetworkManager* networkManager;
	std::map<unsigned, PlayerServer *> playermap;
	GameObjectContainer<GameObject> activeCells;
	GameObjectContainer<GameObject> inactiveCells;
	GameObjectContainer<GameObject> environment;
	boost::mutex mutex;
};