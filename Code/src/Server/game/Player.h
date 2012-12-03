#pragma once

#include <string>
#include <queue>

#include "../network/NetworkMessage.h"
#include "Population.h"

using namespace std;

class Player
{
friend class NetworkManager;

private:
	unsigned int m_uiId;
	string m_strName;
	string m_strIpAddress;
	unsigned short m_usPort;
	Population m_population;

	unsigned m_uiServerPacketId;
	unsigned m_uiClientPacketId;

	queue<unsigned> m_unreceivedMessages;
	queue<NetworkMessage> m_unconfirmedMessages;

public:

	Player(unsigned int id, string name, string ipAddresse, unsigned short port, float position[2]) :
	  m_uiId(id),
	  m_strName(name),
	  m_strIpAddress(ipAddresse),
	  m_usPort(port),
	  m_uiServerPacketId(0),
	  m_uiClientPacketId(0)
	{
		Cell* stemcell = new StemCell();
		m_population.createCell(stemcell);
	}

	unsigned int getId() const { return m_uiId; }
	const string& getName() const { return m_strName; }
	const string& getIpAddress() const { return m_strIpAddress; }
	unsigned short getPort() const { return m_usPort; }
	Population& getPopulation() { return m_population; }
};