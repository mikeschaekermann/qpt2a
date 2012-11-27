#pragma once

#include <string>

#include "Population.h"

using namespace std;

class Player
{
private:
	unsigned int m_uiId;
	string m_strName;
	string m_strIpAddress;
	unsigned short m_usPort;
	Population m_population;

	unsigned int createCellId()
	{
		return m_population.size();
	}

public:

	Player(unsigned int id, string name, string ipAddresse, unsigned short port, float position[2]) :
	  m_uiId(id),
	  m_strName(name),
	  m_strIpAddress(ipAddresse),
	  m_usPort(port)
	{
		Cell* stemcell = new Cell(createCellId(), position);
		m_population.createCell(stemcell);
	}

	unsigned int getId() const { return m_uiId; }
	const string& getName() const { return m_strName; }
	const string& getIpAddress() const { return m_strIpAddress; }
	unsigned short getPort() const { return m_usPort; }
	Population& getPopulation() { return m_population; }
};