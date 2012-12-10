#pragma once

#include <vector>

#include "cinder/Vector.h"

#include "../../common/network/NetworkManager.h"
#include "../../common/network/messages/game/ingame/cell/creation/CreateCellFailure.h"
#include "../../common/network/messages/game/ingame/cell/creation/CellNew.h"
#include "../../common/network/messages/game/ingame/cell/creation/CreateCellSuccess.h"

#include "../event/Event.h"
#include "../event/EventQueue.h"
#include "../event/AttackEvent.h"
#include "../event/BuildingEvent.h"

#include "Player.h"
#include "Cell.h"
#include "StandardCell.h"

class EventCreator
{
public:
	static EventCreator& getInstance();

	void bind(NetworkManager* networkmanager, EventQueue* eventqueue, vector<Player*>* players);

	bool createBuildEvent(const double time, const unsigned int requestId, const int type, const float angle, Player& currentPlayer, Cell& cell);

	bool createAttackEvent(const double time, const bool isAttacker, const Player& currentPlayer, Cell& currentCell);
private:
	static EventCreator* m_pInstance;
	NetworkManager* m_pNetworkManager;
	EventQueue* m_pEventQueue;
	vector<Player*>* m_pPlayers;

	EventCreator();
};