#pragma once

class CellServer;
class PlayerServer;

#include <vector>
#include <map>

#include "GameEvent.h"


class BuildingEvent : public GameEvent
{
public:
	BuildingEvent(double startTime, CellServer & cell);

	virtual void trigger();
private:
	CellServer & cell;
};