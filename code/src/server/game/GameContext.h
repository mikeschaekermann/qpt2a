#pragma once

#include <map>

class PlayerServer;

#define GAMECONTEXT GameContext::getInstance()

class GameContext
{
public:
	static GameContext * getInstance();

	PlayerServer * getPlayer(unsigned id);
	void addPlayer(PlayerServer * player);
	std::map<unsigned, PlayerServer *> & getPlayerMap();

private:
	static GameContext *instance;

	GameContext();
	std::map<unsigned, PlayerServer *> playermap;
};