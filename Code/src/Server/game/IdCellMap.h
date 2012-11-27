#pragma once

#include <map>

#include "ICellMap.h"
#include "Cell.h"

using namespace std;

class IdCellMap : public ICellMap
{
public:
	virtual bool addCell(Cell* cell)
	{
		return m_map.insert(pair<unsigned int, Cell*>(generateKey(), cell)).second;
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

	unsigned int generateKey() const
	{
		return size();
	}
};