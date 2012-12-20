#pragma once

#include <vector>

#include "cinder/Vector.h"

#include "../../common/network/NetworkManager.h"
#include "../../common/network/messages/game/ingame/cell/creation/CreateCellFailure.h"
#include "../../common/network/messages/game/ingame/cell/creation/CellNew.h"
#include "../../common/network/messages/game/ingame/cell/creation/CreateCellSuccess.h"

#include "../../common/GameObjectContainer.h"

#include "../event/Event.h"
#include "../event/EventQueue.h"
#include "../event/AttackEvent.h"
#include "../event/BuildingEvent.h"

#include "PlayerServer.h"
#include "CellServer.h"
#include "StandardCellServer.h"

class EventCreator
{
public:
	static EventCreator * getInstance();

	void bind(NetworkManager * networkManager, EventQueue * eventQueue, GameObjectContainer * gameObjectContainer, vector<PlayerServer *> * players);

	bool createBuildEvent(const double time, const unsigned int requestId, const int type, const float angle, PlayerServer & currentPlayer, CellServer & cell);

	bool createAttackEvent(const double time, const bool isAttacker, const PlayerServer & currentPlayer, CellServer & currentCell);
private:
	static EventCreator * instance;
	NetworkManager * networkManager;
	EventQueue * eventQueue;
	GameObjectContainer * gameObjectContainer;
	vector<PlayerServer *> * players;

	EventCreator();
	EventCreator(const EventCreator & cpy);
};