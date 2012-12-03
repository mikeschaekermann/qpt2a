#pragma once

#include <cmath>
#include <vector>
#include <map>
#include <string>
#include <sstream>

#include "ICellMap.h"
#include "Cell.h"

using namespace std;

class PositionCellMap : public ICellMap
{
public:
	virtual Cell* addCell(Cell* cell)
	{
		pair<map<float, map<float, Cell*> >::iterator, bool> insertResult = 
			m_map.insert(pair<float, map<float, Cell*> >(cell->getPosition()[0], map<float, Cell*>()));
		pair<map<float, Cell*>::iterator, bool> insertResult2 = 
			insertResult.first->second.insert(pair<float, Cell*>(cell->getPosition()[1], cell));
		if (insertResult2.second)
		{
			return insertResult2.first->second;
		}
		return 0;
	}

	virtual void removeCell(Cell* cell)
	{
		map<float, map<float, Cell*> >::iterator find = m_map.find(cell->getPosition()[0]);
		if (find != m_map.end())
		{
			if (find->second.erase(cell->getPosition()[1]))
			{
				/// worked
			}
		}
	}

	Cell* find(const float position[2]) const
	{
		map<float, map<float, Cell*> >::const_iterator cellXIt = m_map.find(position[0]);
		if (cellXIt != m_map.end())
		{
			map<float, Cell*>::const_iterator cellYIt = cellXIt->second.find(position[1]);
			if (cellYIt != cellXIt->second.end())
			{
				return cellYIt->second;
			}
		}
		return 0;
	}

	const vector<Cell*> findInRadiusOf(const float position[2], float radius) const
	{
		vector<Cell*> cells;
		map<float, map<float, Cell*> >::const_iterator startXIt = m_map.lower_bound(position[0]);
		for (map<float, map<float, Cell*> >::const_iterator xIt = startXIt; xIt != m_map.begin(); --xIt)
		{
			map<float, Cell*>::const_iterator yIt = xIt->second.lower_bound(position[1]);
			for (; yIt != xIt->second.begin(); --yIt)
			{
				if (sqrtf(xIt->first * xIt->first + yIt->first * yIt->first) <= radius)
				{
					cells.push_back(yIt->second);
				}
				else
				{
					continue;
				}
			}
		}

		++startXIt;
		for (map<float, map<float, Cell*> >::const_iterator xIt = startXIt; xIt != m_map.end(); ++xIt)
		{
			map<float, Cell*>::const_iterator yIt = xIt->second.upper_bound(position[1]);
			for (; yIt != xIt->second.end(); ++yIt)
			{
				if (sqrtf(xIt->first * xIt->first + yIt->first * yIt->first) <= radius)
				{
					cells.push_back(yIt->second);
				}
				else
				{
					continue;
				}
			}
		}

		return cells;
	}

	virtual unsigned int size() const
	{
		unsigned int size = 0;
		map<float, map<float, Cell*> >::const_iterator it = m_map.begin();
		for (; it != m_map.end(); ++it)
		{
			size += it->second.size();
		}
		return size;
	}
private:
	map<float, map<float, Cell*> > m_map;
};