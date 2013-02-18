#pragma once

class CellServer;

#include <vector>

#include "../../common/GameObjectContainer.h"
#include "GameEvent.h"

class PolypeptideFightEvent : public GameEvent
{
public:
	PolypeptideFightEvent(double startTime, unsigned int cellId1, unsigned int cellId2, unsigned int polypeptideId1, unsigned int polypeptideId2);

	virtual void trigger();
private:
	unsigned int cellId1;
	unsigned int cellId2;
	unsigned int polypeptideId1;
	unsigned int polypeptideId2;
};