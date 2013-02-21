#include "GameContext.h"
#include "PlayerServer.h"
#include "../network/ServerNetworkManager.h"

GameContext * GameContext::instance = 0;

GameObjectContainer<GameObject> & GameContext::getActiveCells()
{
	return activeCells;
}

GameObjectContainer<GameObject> & GameContext::getInactiveCells()
{
	return inactiveCells;
}

GameObjectContainer<GameObject> & GameContext::getEnvironment()
{
	return environment;
}

AttackRelationContainer & GameContext::getAttackRelations()
{
	return attackRelations;
}

PlayerServer * GameContext::getPlayer(unsigned id)
{
	if (playermap.find(id) != playermap.end())
	{
		return playermap[id];
	}

	return 0;
}

void GameContext::addPlayer(PlayerServer * player)
{
	playermap.insert(make_pair(player->getId(), player));
}

std::map<unsigned, PlayerServer *> & GameContext::getPlayerMap()
{
	return playermap;
}

PolypeptideCapacityContainer * GameContext::getPolypeptideCapacityContainer(unsigned int id)
{
	if (polypeptideCapacityContainers.find(id) != polypeptideCapacityContainers.end())
	{
		return polypeptideCapacityContainers[id];
	}

	return nullptr;
}

void GameContext::addPolypeptideCapacityContainer(unsigned int id, PolypeptideCapacityContainer * polypeptideCapacityContainer)
{
	polypeptideCapacityContainers.insert(make_pair(id, polypeptideCapacityContainer));
}

std::map<unsigned int, PolypeptideCapacityContainer *> & GameContext::getPolypeptideCapacityContainer()
{
	return polypeptideCapacityContainers;
}


GameContext * GameContext::getInstance()
{
	if (!instance)
	{
		instance = new GameContext();
	}
	return instance;
}
	
NetworkManager * GameContext::getNetworkManager()
{
	return networkManager;
}

void GameContext::initializeNetworkManager(int port, Game * game)
{
	networkManager = new ServerNetworkManager(port, game);
}

boost::mutex & GameContext::getMutex()
{
	return mutex;
}
	
GameContext::GameContext() : networkManager(0)
{

}