#pragma once

class NetworkManager;
class PlayerServer;
class CellServer;

#include <vector>

#include "cinder/Vector.h"

#define EVENT_CRTR EventCreator::getInstance()

class EventCreator
{
public:
	static EventCreator * getInstance();

	void bind(NetworkManager * networkManager);

	bool createBuildEvent(const double time, const unsigned int requestId, const int type, const float angle, PlayerServer & currentPlayer, CellServer & parentCell, CellServer & cell);

	bool createAttackEvent(const double time, bool isAttacker, const PlayerServer & currentPlayer, CellServer & currentCell);

	float calculateDamage(CellServer * attacker, CellServer * victim);


private:
	static EventCreator * instance;

	
	float getAttackerMultiplier(CellServer * attacker);
	float getVictimMultiplier(CellServer * victim);

	bool checkOtherCells(unsigned int requestId, CellServer & cell, const PlayerServer & player);
	bool checkInWorldRadius(unsigned int requestId, CellServer & cell, const PlayerServer & player);
	void calculateStaticEffects(CellServer & cell);
	void sendCellCreationMessages(unsigned int requestId, PlayerServer & currentPlayer, CellServer & cell, const int type);

	EventCreator();
	EventCreator(const EventCreator & cpy);
};