#pragma once

#include <vector>

#include "Event.h"

#include "../game/EventCreator.h"
#include "../game/CellServer.h"
#include "../game/PlayerServer.h"

#include "../../common/network/NetworkManager.h"
#include "../../common/network/messages/game/ingame/cell/creation/CreateCellComplete.h"

class BuildingEvent : public GameEvent
{
public:
	BuildingEvent(double startTime, NetworkManager & manager, CellServer & cell, const vector<PlayerServer *> & players);

	virtual void trigger();
private:
	NetworkManager & manager;
	CellServer & cell;
	const vector<PlayerServer *> & players;
};