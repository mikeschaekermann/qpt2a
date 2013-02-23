#pragma once
#ifndef _WIN32_WINNT
	#define _WIN32_WINNT 0x0501
#endif

#include <list>
#include <map>
#include <queue>
#include <boost/asio.hpp>

#include "messages/NetworkMessage.h"

struct CompareNetworkMessage : public std::binary_function<NetworkMessage *, NetworkMessage *, bool>                                                                                       
{  
  bool operator()(const NetworkMessage * lhs, const NetworkMessage * rhs) const  
  {  
	  return lhs->messageId > rhs->messageId;  
  }  
};

class ConnectionEndpoint
{
public:
	unsigned m_uiLocalPacketId;
	unsigned m_uiRemotePacketId;
	
	std::list<unsigned int> m_unreceivedMessages;
	std::map<unsigned int, NetworkMessage*> m_unconfirmedMessages;
	std::priority_queue<NetworkMessage *, std::vector<NetworkMessage*>, CompareNetworkMessage> m_unhandledMessages;
	
	boost::asio::ip::udp::endpoint m_endpoint;

	ConnectionEndpoint(boost::asio::ip::udp::endpoint endpoint);
};