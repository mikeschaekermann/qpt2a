#pragma once

class CellServer;

#include <vector>

#include "../../common/GameObjectContainer.h"
#include "GameEvent.h"

class AttackEvent : public GameEvent
{
public:
	AttackEvent(double startTime, unsigned int attackerId, unsigned int attackedId, float damage);

	virtual void trigger();
private:
	unsigned int attackerId;
	unsigned int attackedId;
	float damage;
};