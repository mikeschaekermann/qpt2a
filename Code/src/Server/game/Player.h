#pragma once

#include <string>
#include <vector>
#include <map>

#include "../../common/network/ConnectionEndpoint.h"
#include "Population.h"

using namespace std;

class Player : public ConnectionEndpoint
{

private:
	unsigned int m_uiId;
	string m_strName;
	Population m_population;
	
public:

	Player(unsigned int id, string name, boost::asio::ip::udp::endpoint endpoint, float position[2]) :
	  m_uiId(id),
	  m_strName(name),
	  ConnectionEndpoint(endpoint)
	{
		Cell* stemcell = new StemCell();
		m_population.createCell(stemcell);
	}

	unsigned int getId() const { return m_uiId; }
	const string& getName() const { return m_strName; }
	const boost::asio::ip::udp::endpoint& getEndpoint() const { return m_endpoint; }
	Population& getPopulation() { return m_population; }
};