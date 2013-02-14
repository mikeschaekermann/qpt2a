#pragma once

class CellServer;

#include <vector>

#include "../../common/GameObjectContainer.h"
#include "GameEvent.h"

class CellDieEvent : public GameEvent
{
public:
	CellDieEvent(double startTime, unsigned int cellId);

	virtual void trigger();
private:
	unsigned int cellId;
};