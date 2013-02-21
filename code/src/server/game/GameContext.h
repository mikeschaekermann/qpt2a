#pragma once

#include <boost\thread\mutex.hpp>

#include "AttackRelationContainer.h"

#include "../../common/GameObject.h"
#include "../../common/GameObjectContainer.h"
#include "../../common/PolypeptideCapacityContainer.h"

#include <map>

class PlayerServer;
class NetworkManager;
class Game;

#define GAMECONTEXT GameContext::getInstance()
#define NETWORKMANAGER GameContext::getInstance()->getNetworkManager()
#define POLYCAPACITY(x) GAMECONTEXT->getPolypeptideCapacityContainer(x)

class GameContext
{
public:
	static GameContext * getInstance();

	GameObjectContainer<GameObject> & getActiveCells();
	GameObjectContainer<GameObject> & getInactiveCells();
	GameObjectContainer<GameObject> & getEnvironment();
	AttackRelationContainer & getAttackRelations();

	PlayerServer * getPlayer(unsigned id);
	void addPlayer(PlayerServer * player);
	std::map<unsigned, PlayerServer *> & getPlayerMap();

	PolypeptideCapacityContainer * getPolypeptideCapacityContainer(unsigned int id);
	void addPolypeptideCapacityContainer(unsigned int id, PolypeptideCapacityContainer * polypeptideCapacityContainer);
	std::map<unsigned int, PolypeptideCapacityContainer *> & getPolypeptideCapacityContainer();

	NetworkManager * getNetworkManager();
	void initializeNetworkManager(int port, Game * game);

	boost::mutex & getMutex();

private:
	// Singleton stuff
	static GameContext *instance;
	GameContext();

	NetworkManager* networkManager;
	std::map<unsigned, PlayerServer *> playermap;
	/// identified by player id
	std::map<unsigned int, PolypeptideCapacityContainer *> polypeptideCapacityContainers;
	GameObjectContainer<GameObject> activeCells;
	GameObjectContainer<GameObject> inactiveCells;
	GameObjectContainer<GameObject> environment;
	AttackRelationContainer attackRelations;
	boost::mutex mutex;
};