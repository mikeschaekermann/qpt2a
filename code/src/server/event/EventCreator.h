#pragma once

class NetworkManager;
class GameObjectContainer;
class PlayerServer;
class CellServer;

#include <vector>

#include "cinder/Vector.h"

#define EVENT_CRTR EventCreator::getInstance()

class EventCreator
{
public:
	static EventCreator * getInstance();

	void bind(NetworkManager * networkManager, GameObjectContainer * gameObjectContainer, std::vector<PlayerServer *> * players);

	bool createBuildEvent(const double time, const unsigned int requestId, const int type, const float angle, PlayerServer & currentPlayer, CellServer & cell);

	bool createAttackEvent(const double time, const bool isAttacker, const PlayerServer & currentPlayer, CellServer & currentCell);
private:
	static EventCreator * instance;
	NetworkManager * networkManager;
	GameObjectContainer * gameObjectContainer;
	std::vector<PlayerServer *> * players;

	EventCreator();
	EventCreator(const EventCreator & cpy);
};