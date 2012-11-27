/**
* Tree Structure for Cells
* Every Cell is for itself a tree
*/

#pragma once

#include <cmath>
#include <string>
#include <sstream>
#include <map>
#include <vector>

#include "Cell.h"
#include "IdCellMap.h"
#include "PositionCellMap.h"

using namespace std;

class Population
{
public:
	bool createCell(Cell* cell)
	{
		if (m_positionMap.addCell(cell))
		{
			return m_idMap.addCell(cell);
		}
		return false;
	}

	void createCell(Cell* cell, const float position[2])
	{
		cell->m_afPosition[0] = position[0];
		cell->m_afPosition[1] = position[1];
		createCell(cell);
	}

	const float* const getRelativePosition(unsigned int cellId, float angle) const
	{
		Cell* cell = m_idMap.find(cellId);
		if (cell != 0)
		{
			float* position = new float[2];
			position[0] = cell->getPosition()[0] + cosf(angle * (float)M_PI / 180.f) * cell->m_fRadius;
			position[1] = cell->getPosition()[1] + sinf(angle * (float)M_PI / 180.f) * cell->m_fRadius;
			return position;
		}
		return 0;
	}

	Cell* find(unsigned int cellId) const
	{
		return m_idMap.find(cellId);
	}

	Cell* find(const float position[2]) const
	{
		return m_positionMap.find(position);
	}

	const vector<Cell*> findInRadiusOf(const float position[2], float radius) const
	{
		return m_positionMap.findInRadiusOf(position, radius);
	}

	unsigned int size() const
	{
		if (m_idMap.size() != m_positionMap.size())
		{
			/// something wicked happens here
		}
		return m_idMap.size();
	}
private:
	IdCellMap m_idMap;
	PositionCellMap m_positionMap;
};