#include "AttackRelationContainer.h"
#include "GameContext.h"
#include "../event/EventManager.h"
#include "../event/EventCreator.h"

using namespace std;

AttackRelationContainer & AttackRelationContainer::addRelation(CellServer & cell1, CellServer & cell2)
{
	addRelationKeyElement(cell1.getId(), cell2.getId());
	auto & elements = addRelationKeyElement(cell2.getId(), cell1.getId());
	relations.insert(make_pair(elements, Relation()));

	return *this;
}

AttackRelationContainer & AttackRelationContainer::removeRelationsWith(CellServer & cell)
{
	removeRelationsWith(cell.getId());

	return *this;
}

AttackRelationContainer & AttackRelationContainer::update()
{
	resetRelations();
	makeRelations();
	refillRelations();

	return *this;
}

set<unsigned int> AttackRelationContainer::make_set(unsigned int key1, unsigned int key2)
{
	set<unsigned int> key;
	key.insert(key1);
	key.insert(key2);
	return key;
}

void AttackRelationContainer::resetRelations()
{
	for (auto it = relations.begin(); it != relations.end(); ++it)
	{
		it->second.polypeptideIds1.clear();
		it->second.polypeptideIds2.clear();

		for (auto eIt = it->second.events.begin(); eIt != it->second.events.end(); ++eIt)
		{
			(*eIt)->setTerminated();
		}
		it->second.events.clear();
	}
}

void AttackRelationContainer::makeRelations()
{
	for (auto keyIt = relationKey.begin(); keyIt != relationKey.end(); ++keyIt)
	{
		auto cellId = keyIt->first;

		CellServer * cell = dynamic_cast<CellServer *>(GAMECONTEXT->getActiveCells().find(cellId));
		if (cell == nullptr)
		{
			/// something is wrong
			/// remove cell from container
			//removeRelationsWith(keyIt->first); -> iterator problem pointing somewhere else
		}

		auto elementsIt = keyIt->second.begin();		
		for (auto polyIt = cell->getPolypeptides().begin(); polyIt != cell->getPolypeptides().end(); ++polyIt, ++elementsIt)
		{
			auto polyId = polyIt->second->getId();

			/// for circular loop
			if (elementsIt == keyIt->second.end()) elementsIt = keyIt->second.begin();

			auto relationIt = relations.find(make_set(cellId, *elementsIt));
			if (relationIt != relations.end())
			{
				auto & relation = relationIt->second;
				auto polypeptideIds = relation[cellId];
				if (polypeptideIds != nullptr)
				{
					polypeptideIds->insert(polyId);
				}
			}
		}
	}
}

void AttackRelationContainer::refillRelations()
{
	for (auto it = relations.begin(); it != relations.end(); ++it)
	{
		auto & polypeptideIds1 = it->second.polypeptideIds1;
		auto & polypeptideIds2 = it->second.polypeptideIds2;

		auto p1It = polypeptideIds1.begin();
		auto p2It = polypeptideIds2.begin();
		for (;p1It != polypeptideIds1.end() && p2It != polypeptideIds2.end(); ++p1It, ++p2It)
		{
			dynamic_cast<CellServer *>(GAMECONTEXT->getActiveCells().find(it->second.cellId1))
				->getPolypeptides().find(*p1It)->second->setState(Polypeptide::POLYPEPTIDEFIGHT);
			dynamic_cast<CellServer *>(GAMECONTEXT->getActiveCells().find(it->second.cellId2))
				->getPolypeptides().find(*p2It)->second->setState(Polypeptide::POLYPEPTIDEFIGHT);
			
			it->second.events.insert(
				EVENT_CRTR->createPolypeptideFightEvent(
					EVENT_MGR->getTime(),
					it->second.cellId1,
					it->second.cellId2,
					*p1It,
					*p2It));
		}

		if (p1It != polypeptideIds1.end())
		{
			for (; p1It != polypeptideIds1.end(); ++p1It)
			{
				dynamic_cast<CellServer *>(GAMECONTEXT->getActiveCells().find(it->second.cellId1))
					->getPolypeptides().find(*p1It)->second->setState(Polypeptide::CELLFIGHT);
				it->second.events.insert(
					EVENT_CRTR->createPolypeptideCellAttackEvent(
						EVENT_MGR->getTime(),
						it->second.cellId1,
						it->second.cellId2,
						*p1It));
			}
		}
		else
		{
			for (; p2It != polypeptideIds2.end(); ++p2It)
			{
				dynamic_cast<CellServer *>(GAMECONTEXT->getActiveCells().find(it->second.cellId2))
					->getPolypeptides().find(*p2It)->second->setState(Polypeptide::CELLFIGHT);
				it->second.events.insert(
					EVENT_CRTR->createPolypeptideCellAttackEvent(
						EVENT_MGR->getTime(),
						it->second.cellId2,
						it->second.cellId1,
						*p2It));
			}
		}
	}
}

void AttackRelationContainer::removeRelationsWith(unsigned int cellId)
{
	auto & keyIt = relationKey.find(cellId);
	if (keyIt != relationKey.end())
	{
		for (auto it = keyIt->second.begin(); it != keyIt->second.end(); ++it)
		{
			relations.erase(make_set(cellId, *it));
		}
	}
	relationKey.erase(cellId);
}

set<unsigned int> & AttackRelationContainer::addRelationKeyElement(unsigned int key, unsigned int element)
{
	relationKey.insert(make_pair(key, set<unsigned int>()));
	auto & elements = relationKey.find(key)->second;
	elements.insert(element);

	return elements;
}