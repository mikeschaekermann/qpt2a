#include "GameContext.h"
#include "PlayerServer.h"

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

GameContext * GameContext::getInstance()
{
	if (!instance)
	{
		instance = new GameContext();
	}
	return instance;
}
	
	
GameContext::GameContext()
{

}