/**
* Tree Structure for Cells
* Every Cell is for itself a tree
*/

#pragma once

#include <map>
#include <vector>

#include "Cell.h"

using namespace std;

class Population
{
private:
	class CellRelation
	{
	public:
		Cell* c;
		vector<Cell*> parents;
		vector<Cell*> children;
	};

	map<int, CellRelation*> m_idMap;
	map<int, map<int, CellRelation*> > m_positionMap;
public:
	void createCell(Cell* cell)
	{
		CellRelation* relation = new CellRelation();
		relation->c = cell;
		m_idMap.insert(pair<int, CellRelation*>(size(), relation));
	}

	unsigned int size() const
	{
		return m_idMap.size();
	}

	const Cell& getCellById() const
	{
		return *(m_idMap.at(0)->c);
	}
};