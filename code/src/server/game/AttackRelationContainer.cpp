#include "AttackRelationContainer.h"
#include "GameContext.h"
#include "../event/EventManager.h"
#include "../event/EventCreator.h"

using namespace std;

AttackRelationContainer & AttackRelationContainer::addRelation(CellServer & cell1, CellServer & cell2)
{
	createRelation(cell1, cell2);
	updateRelationsFor(cell1);
	updateRelationsFor(cell2);

	return *this;
}

AttackRelationContainer & AttackRelationContainer::removeRelationsFor(CellServer & cell)
{
	removeRelationsFor(cell.getId());

	return *this;
}

AttackRelationContainer & AttackRelationContainer::updateRelationsFor(CellServer & cell)
{
	loopThroughRelations(cell);

	return *this;
}

set<unsigned int> AttackRelationContainer::make_set(unsigned int key1, unsigned int key2)
{
	set<unsigned int> key;
	key.insert(key1);
	key.insert(key2);
	return key;
}

void AttackRelationContainer::createRelation(CellServer & cell1, CellServer & cell2)
{
	auto cellId1 = cell1.getId();
	auto cellId2 = cell2.getId();
	addRelationKeyElement(cellId1, cellId2);
	addRelationKeyElement(cellId2, cellId1);
	auto & key = make_set(cellId1, cellId2);
	auto & relation = relations
		.insert(make_pair(key, Relation(cell1, cell2))).first->second;
}

void AttackRelationContainer::loopThroughRelations(CellServer & cell)
{
	auto & keyIt = relationKey.find(cell.getId());
	if (keyIt != relationKey.end())
	{
		auto elementsIt = keyIt->second.begin();
		if (elementsIt == keyIt->second.end()) return;
		/// loop through polypeptides
		/// polypeptides will be assigned to other cells circular
		for (auto polyIt = cell.getPolypeptides().begin(); polyIt != cell.getPolypeptides().end(); ++polyIt, ++elementsIt)
		{
			auto polyId = polyIt->second->getId();

			/// for circular loop
			if (elementsIt == keyIt->second.end()) elementsIt = keyIt->second.begin();

			/// find iterator of current cell and circular looping cells
			auto & relationIt = relations.find(make_set(cell.getId(), *elementsIt));
			if (relationIt != relations.end())
			{
				auto & relation = relationIt->second;

				resetRelations(relation);

				fillRelations(relation, cell, polyId);

				eventRelations(relation);
			}
		}
	}
}

void AttackRelationContainer::resetRelations(Relation & relation)
{
	relation.polypeptideIds1.clear();
	relation.polypeptideIds2.clear();

	for (auto eIt = relation.events.begin(); eIt != relation.events.end(); ++eIt)
	{
		if (!(EVENT_MGR->isDeletedEvent(eIt->first)))
		{
			eIt->second->setTerminated();
		}
	}
	relation.events.clear();
}

void AttackRelationContainer::fillRelations(Relation & relation, CellServer & cell, unsigned int polyId)
{
	auto polypeptideIds = relation[cell.getId()];
	if (polypeptideIds != nullptr)
	{
		polypeptideIds->insert(polyId);
	}
}

void AttackRelationContainer::eventRelations(Relation & relation)
{
	auto & polypeptideIds1 = relation.polypeptideIds1;
	auto & polypeptideIds2 = relation.polypeptideIds2;

	auto p1It = polypeptideIds1.begin();
	auto p2It = polypeptideIds2.begin();
	for (;p1It != polypeptideIds1.end() && p2It != polypeptideIds2.end(); ++p1It, ++p2It)
	{
		relation.cell1->getPolypeptides().find(*p1It)->second->setState(Polypeptide::POLYPEPTIDEFIGHT);
		relation.cell2->getPolypeptides().find(*p2It)->second->setState(Polypeptide::POLYPEPTIDEFIGHT);
		
		auto e = EVENT_CRTR->createPolypeptideFightEvent(
				EVENT_MGR->getTime(),
				relation.cell1->getId(),
				relation.cell2->getId(),
				*p1It,
				*p2It);
		relation.events.insert(make_pair(e->getId(), e));
	}

	if (p1It != polypeptideIds1.end())
	{
		for (; p1It != polypeptideIds1.end(); ++p1It)
		{
			relation.cell1->getPolypeptides().find(*p1It)->second->setState(Polypeptide::CELLFIGHT);
			
			auto e = EVENT_CRTR->createPolypeptideCellAttackEvent(
					EVENT_MGR->getTime(),
					relation.cell1->getId(),
					relation.cell2->getId(),
					*p1It);
			relation.events.insert(make_pair(e->getId(), e));
		}
	}
	else
	{
		for (; p2It != polypeptideIds2.end(); ++p2It)
		{
			relation.cell2->getPolypeptides().find(*p2It)->second->setState(Polypeptide::CELLFIGHT);

			auto e = EVENT_CRTR->createPolypeptideCellAttackEvent(
					EVENT_MGR->getTime(),
					relation.cell2->getId(),
					relation.cell1->getId(),
					*p2It);
			relation.events.insert(make_pair(e->getId(), e));
		}
	}
}

void AttackRelationContainer::removeRelationsFor(unsigned int cellId)
{
	auto & keyIt = relationKey.find(cellId);
	if (keyIt != relationKey.end())
	{
		for (auto it = keyIt->second.begin(); it != keyIt->second.end(); ++it)
		{
			/// delete relations
			relations.erase(make_set(cellId, *it));
			
			/// delete key in references
			auto & otherKeyIt = relationKey.find(*it);
			if (otherKeyIt != relationKey.end())
			{
				otherKeyIt->second.erase(cellId);
			}
		}
	}
	/// delete key with references
	relationKey.erase(cellId);
}

void AttackRelationContainer::addRelationKeyElement(unsigned int key, unsigned int element)
{
	auto & it = relationKey.insert(make_pair(key, set<unsigned int>())).first;
	auto & elements = it->second;
	elements.insert(element);
}

AttackRelationContainer::Relation::Relation(CellServer & cell1, CellServer & cell2) :
	cell1(&cell1),
	cell2(&cell2)
{ }

AttackRelationContainer::Relation::Relation():
	cell1(nullptr),
	cell2(nullptr)
{ }