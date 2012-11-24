#pragma once

#include <string>

#include "Population.h"

using namespace std;

class Player
{
private:
	unsigned int m_id;
	string m_name;
	string m_ipAddress;
	unsigned short m_port;
	Population m_population;

	unsigned int createCellId()
	{
		return m_population.size();
	}

public:

	Player(unsigned int id, string name, string ipAddresse, unsigned short port, float position[3]) :
	  m_id(id),
	  m_name(name),
	  m_ipAddress(ipAddresse),
	  m_port(port)
	{
		Cell* stemcell = new Cell(createCellId(), position);
		m_population.createCell(stemcell);
	}

	unsigned int getId() const { return m_id; }
	const string& getName() const { return m_name; }
	const string& getIpAddress() const { return m_ipAddress; }
	unsigned short getPort() const { return m_port; }
	const Population& getPopulation() const { return m_population; }
};