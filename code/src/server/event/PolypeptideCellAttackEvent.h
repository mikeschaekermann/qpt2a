#pragma once

class CellServer;

#include <vector>

#include "../../common/GameObjectContainer.h"
#include "GameEvent.h"

class PolypeptideCellAttackEvent : public GameEvent
{
public:
	PolypeptideCellAttackEvent(double startTime, unsigned int attackerCellId, unsigned int attackedCellId, unsigned int polypeptideId, float damage);

	virtual void trigger();
private:
	unsigned int attackerCellId;
	unsigned int attackedCellId;
	unsigned int polypeptideId;
	float damage;
};