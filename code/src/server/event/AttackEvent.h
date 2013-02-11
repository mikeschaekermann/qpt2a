#pragma once

class NetworkManager;
class GameObjectContainer;
class CellServer;
class PlayerServer;

#include <vector>

#include "GameEvent.h"

class AttackEvent : public GameEvent
{
public:
	AttackEvent(double startTime, NetworkManager & manager, GameObjectContainer & gameObjectContainer, CellServer & attacker, CellServer & victim, float damage, std::vector<PlayerServer *> & players);

	virtual void trigger();
private:
	NetworkManager & manager;
	GameObjectContainer<GameObject> & gameObjectContainer;
	CellServer & attacker;
	CellServer & victim;
	float damage;
	std::vector<PlayerServer *> & players;
};