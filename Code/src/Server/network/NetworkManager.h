#pragma once
#define _WIN32_WINNT 0x0501
class Game;
class Player;
#include "NetworkMessage.h"

class NetworkManager
{
public:
	NetworkManager(unsigned short listenPort, Game *game);
	~NetworkManager();
		
	void operator()();
	void send(NetworkMessage message);


protected:
	virtual NetworkMessage* createNetworkMessage(char* data);
	virtual void handleMessage(NetworkMessage* message);

private:
	NetworkManager(const NetworkManager &other);
	Player* getPlayer(boost::asio::ip::udp::endpoint endpoint);

	boost::asio::ip::udp::socket serverSocket;
	boost::asio::io_service io_service;
	Game *game;
};