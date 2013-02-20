#pragma once

#include <map>
#include <set>

#include "CellServer.h"
#include "../event/GameEvent.h"

class AttackRelationContainer
{
public:
	AttackRelationContainer & addRelation(CellServer & cell1, CellServer & cell2);
	AttackRelationContainer & removeRelationsWith(CellServer & cell);
	AttackRelationContainer & update();
private:
	struct Relation
	{
		unsigned int cellId1;
		unsigned int cellId2;
		std::set<unsigned int> polypeptideIds1;
		std::set<unsigned int> polypeptideIds2;
		std::set<GameEvent *> events;

		std::set<unsigned int> * operator[](unsigned int id)
		{
			if (id == cellId1)
				return &polypeptideIds1;
			if (id == cellId2)
				return &polypeptideIds2;

			return nullptr;
		}
	};

	std::set<unsigned int> make_set(unsigned int key1, unsigned int key2);
	void resetRelations();
	void makeRelations();
	void refillRelations();
	void removeRelationsWith(unsigned int cellId);
	void addRelationKeyElement(unsigned int key, unsigned int element);

	bool isUpdated;
	std::map<unsigned int, std::set<unsigned int> > relationKey;
	std::map<set<unsigned int>, Relation> relations;
};