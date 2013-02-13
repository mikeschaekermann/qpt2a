#pragma once

class CellServer;
class PlayerServer;

#include <vector>

#include "GameEvent.h"


class BuildingEvent : public GameEvent
{
public:
	BuildingEvent(double startTime, CellServer & cell);

	virtual void trigger();
private:
	CellServer & cell;
};