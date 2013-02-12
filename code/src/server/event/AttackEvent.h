#pragma once

class CellServer;

#include <vector>

#include "../../common/GameObjectContainer.h"
#include "GameEvent.h"

class AttackEvent : public GameEvent
{
public:
	AttackEvent(double startTime, CellServer & attacker, CellServer & victim, float damage);

	virtual void trigger();
private:
	CellServer & attacker;
	CellServer & victim;
	float damage;
};