#pragma once

#include <string>
#include <vector>
#include <map>

#include "../network/NetworkMessage.h"
#include "Population.h"

using namespace std;

class Player
{
friend class NetworkManager;

private:
	unsigned int m_uiId;
	string m_strName;
	boost::asio::ip::udp::endpoint m_endpoint;
	Population m_population;

	unsigned m_uiServerPacketId;
	unsigned m_uiClientPacketId;

	list<unsigned> m_unreceivedMessages;
	map<unsigned, NetworkMessage> m_unconfirmedMessages;

public:

	Player(unsigned int id, string name, boost::asio::ip::udp::endpoint endpoint, float position[2]) :
	  m_uiId(id),
	  m_strName(name),
	  m_endpoint(endpoint),
	  m_uiServerPacketId(0),
	  m_uiClientPacketId(0)
	{
		Cell* stemcell = new StemCell();
		m_population.createCell(stemcell);
	}

	unsigned int getId() const { return m_uiId; }
	const string& getName() const { return m_strName; }
	const boost::asio::ip::udp::endpoint& getEndpoint() const { return m_endpoint; }
	Population& getPopulation() { return m_population; }
};