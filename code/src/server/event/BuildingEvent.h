#pragma once

class NetworkManager;
class CellServer;
class PlayerServer;

#include <vector>

#include "GameEvent.h"


class BuildingEvent : public GameEvent
{
public:
	BuildingEvent(double startTime, NetworkManager & manager, CellServer & cell, const std::vector<PlayerServer *> & players);

	virtual void trigger();
private:
	NetworkManager & manager;
	CellServer & cell;
	const std::vector<PlayerServer *> & players;
};