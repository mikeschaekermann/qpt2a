#pragma once

#include <map>
#include <set>

#include "CellServer.h"
#include "../event/GameEvent.h"

class AttackRelationContainer
{
public:
	AttackRelationContainer & addRelation(CellServer & cell1, CellServer & cell2);
	AttackRelationContainer & removeRelationsFor(CellServer & cell);
	AttackRelationContainer & updateRelationsFor(CellServer & cell);
private:
	struct Relation
	{
		CellServer & cell1;
		CellServer & cell2;
		std::set<unsigned int> polypeptideIds1;
		std::set<unsigned int> polypeptideIds2;
		std::set<GameEvent *> events;
		
		Relation(CellServer & cell1, CellServer & cell2);

		std::set<unsigned int> * operator[](unsigned int id)
		{
			if (id == cell1.getId())
				return &polypeptideIds1;
			if (id == cell2.getId())
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
};