#pragma once
#ifndef _WIN32_WINNT
	#define _WIN32_WINNT 0x0501
#endif

#include <list>
#include <map>
#include <boost/asio.hpp>

#include "messages/NetworkMessage.h"

class ConnectionEndpoint
{
public:
	unsigned m_uiLocalPacketId;
	unsigned m_uiRemotePacketId;
	
	std::list<unsigned int> m_unreceivedMessages;
	std::map<unsigned int, NetworkMessage*> m_unconfirmedMessages;
	
	boost::asio::ip::udp::endpoint m_endpoint;

	ConnectionEndpoint(boost::asio::ip::udp::endpoint endpoint);
};