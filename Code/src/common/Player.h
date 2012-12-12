#pragma once

#include "../common/Config.h"

#include <string>
#include <vector>
#include <map>

#include "StemCell.h"
#include "Population.h"

using namespace std;

class Player
{

private:
	unsigned int m_uiId;
	string m_strName;
	Population m_population;
	
public:

	Player(unsigned int id, string name, Vec2f position) :
	  m_uiId(id),
	  m_strName(name)
	{
		Cell* stemcell = new StemCell();
		m_population.createCell(stemcell, position, 0.f);
	}

	unsigned int getId() const { return m_uiId; }
	const string& getName() const { return m_strName; }
	Population& getPopulation() { return m_population; }
	const Population& getConstPopulation() const { return m_population; }
};