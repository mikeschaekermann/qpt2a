#pragma once

#include <map>

#include "ICellMap.h"
#include "Cell.h"

using namespace std;

class IdCellMap : public ICellMap
{
public:
	virtual Cell* addCell(Cell* cell)
	{
		cell->setId(generateKey());
		pair<map<unsigned int, Cell*>::iterator, bool> insertResult = m_map.insert(pair<unsigned int, Cell*>(cell->getId(), cell));
		if (insertResult.second)
		{
			return insertResult.first->second;
		}
		return 0;
	}

	virtual void removeCell(Cell* cell)
	{
		if (m_map.erase(cell->getId()))
		{
			/// worked
		}
	}

	Cell* find(unsigned int id) const
	{
		map<unsigned int, Cell*>::const_iterator it = m_map.find(id);
		if (it != m_map.end())
		{
			return it->second;
		}
		return 0;
	}

	virtual unsigned int size() const
	{
		return m_map.size();
	}
private:
	map<unsigned int, Cell*> m_map;
	static unsigned int m_uiMaxCellId;

	unsigned int generateKey() const
	{
		return ++m_uiMaxCellId;
	}
};