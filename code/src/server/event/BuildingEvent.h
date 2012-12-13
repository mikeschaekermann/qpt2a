#pragma once

#include <vector>

#include "Event.h"

#include "../game/EventCreator.h"
#include "../game/Cell.h"
#include "../game/Player.h"

#include "../../common/network/NetworkManager.h"
#include "../../common/network/messages/game/ingame/cell/creation/CreateCellComplete.h"

#define BUILDING_EVENT_DURATION 30.f

class BuildingEvent : public Event
{
public:
	BuildingEvent(double startTime, NetworkManager& manager, Cell& cell, const vector<Player*>& players);

	virtual void trigger();
private:
	NetworkManager& m_rManager;
	Cell& m_rCell;
	const vector<Player*>& m_rPlayers;
};