#pragma once

class NetworkManager;
class PlayerServer;
class CellServer;
class PolypeptideServer;
class GameEvent;

#include <vector>

#include "cinder/Vector.h"

#define EVENT_CRTR EventCreator::getInstance()

class EventCreator
{
public:
	static EventCreator * getInstance();

	void bind(NetworkManager * networkManager);

	bool createBuildEvent(double time, const unsigned int requestId, const int type, const float angle, PlayerServer & currentPlayer, CellServer & parentCell, CellServer & cell);

	bool createAttackEvent(double time, bool isAttacker, CellServer & currentCell);

	GameEvent * createPolypeptideFightEvent(double startTime, unsigned int cellId1, unsigned int cellId2, unsigned int polypeptideId1, unsigned int polypeptideId2);

	GameEvent * createPolypeptideCellAttackEvent(double startTime, unsigned int attackerCellId, unsigned int attackedCellId, unsigned int polypeptideId);
private:
	static EventCreator * instance;

	float calculateDamage(CellServer * attacker, CellServer * victim);
	
	float getAttackerMultiplier(CellServer * attacker);
	float getVictimMultiplier(CellServer * victim);

	bool checkCollision(unsigned int requestId, CellServer & cell, const PlayerServer & player);
	bool checkInWorldRadius(unsigned int requestId, CellServer & cell, const PlayerServer & player);
	void calculateStaticEffects(CellServer & cell);
	void sendCellCreationMessages(unsigned int requestId, PlayerServer & currentPlayer, CellServer & cell, const int type);

	void createPolypeptideCellAttackEvent(double time, unsigned int attackerCellId, unsigned int attackedCellId, std::vector<unsigned int> & polypeptideIds, float damage);

	EventCreator();
	EventCreator(const EventCreator & cpy);
};