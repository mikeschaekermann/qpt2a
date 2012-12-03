#pragma once

class Game;
#include "../../common/network/NetworkManager.h"

class ServerNetworkManager : public NetworkManager
{
public:
	ServerNetworkManager(unsigned short listenPort, Game *game);

protected:
	virtual ConnectionEndpoint* getConnectionEndpoint(boost::asio::ip::udp::endpoint endpoint);
	virtual NetworkMessage* createNetworkMessage(char* data);
	virtual void handleMessage(NetworkMessage* message);
	virtual std::vector<ConnectionEndpoint> getConnectionEndpoints();
private:
	ServerNetworkManager(const ServerNetworkManager &other);
	Game *m_game;
};