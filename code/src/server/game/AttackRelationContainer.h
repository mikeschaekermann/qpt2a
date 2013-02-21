#pragma once

#include <map>
#include <set>

#include "CellServer.h"
#include "../event/GameEvent.h"
#include "../event/PolypeptideCellAttackEvent.h"
#include "../event/PolypeptideFightEvent.h"
#include <boost/thread/mutex.hpp>

class AttackRelationContainer
{
	friend class PolypeptideCellAttackEvent;
	friend class PolypeptideFightEvent;
public:
	AttackRelationContainer & addRelation(CellServer & cell1, CellServer & cell2);
	AttackRelationContainer & removeRelationsFor(CellServer & cell);
	AttackRelationContainer & updateRelationsFor(CellServer & cell);

	void removeEvent(unsigned int id);
private:
	struct Relation
	{
		CellServer * cell1;
		CellServer * cell2;
		std::set<unsigned int> polypeptideIds1;
		std::set<unsigned int> polypeptideIds2;
		std::set<unsigned int> eventIds;
		
		Relation();
		Relation(CellServer & cell1, CellServer & cell2);

		std::set<unsigned int> * operator[](unsigned int id)
		{
			if (id == cell1->getId())
				return &polypeptideIds1;
			if (id == cell2->getId())
				return &polypeptideIds2;

			return nullptr;
		}
	};

	std::set<unsigned int> make_set(unsigned int key1, unsigned int key2);

	void createRelation(CellServer & cell1, CellServer & cell2);

	void loopThroughRelations(CellServer & cell);

	void resetRelations(Relation & relation);
	void fillRelations(Relation & relation, CellServer & cell, unsigned int polyId);
	void eventRelations(Relation & relation);

	void removeRelationsFor(unsigned int cellId);
	void addRelationKeyElement(unsigned int key, unsigned int element);

	std::map<unsigned int, std::set<unsigned int> > relationKey;
	std::map<set<unsigned int>, Relation> relations;
	std::map<unsigned int, GameEvent *> events;
	boost::mutex mutex;
};