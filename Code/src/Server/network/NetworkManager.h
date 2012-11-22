#pragma once
#define _WIN32_WINNT 0x0501
#include "NetworkMessage.h"

class NetworkManager
{
public:
	NetworkManager(unsigned short listenPort);
	~NetworkManager();
		
	void operator()();
	void send(NetworkMessage message);

private:
	boost::asio::ip::udp::socket serverSocket;
	boost::asio::io_service io_service;
};