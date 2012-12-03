#pragma once

#include "../../common/network/NetworkManager.h"

using namespace boost::asio::ip;

class ClientNetworkManager : public NetworkManager
{
public:
	ClientNetworkManager(udp::endpoint serverEndpoint);

protected:
	virtual ConnectionEndpoint* getConnectionEndpoint(boost::asio::ip::udp::endpoint endpoint);
	virtual NetworkMessage* createNetworkMessage(char* data);
	virtual void handleMessage(NetworkMessage* message);
	virtual std::vector<ConnectionEndpoint> getConnectionEndpoints();
private:
	ClientNetworkManager(const ClientNetworkManager &other);

	udp::endpoint m_serverEndpoint;
	std::vector<ConnectionEndpoint> m_endpoints;
};