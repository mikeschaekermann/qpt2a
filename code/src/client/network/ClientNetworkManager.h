#pragma once

#include "../../common/network/NetworkManager.h"
#include <map>

using namespace boost::asio::ip;

class CreateCellRequest;
class CellClient;

class ClientNetworkManager : public NetworkManager
{
public:
	ClientNetworkManager(udp::endpoint serverEndpoint);
	
	virtual void registerCreateCellRequest(
		CreateCellRequest * request,
		CellClient * newCell,
		CellClient * parentCell
	);
	
protected:
	virtual ConnectionEndpoint* getConnectionEndpoint(boost::asio::ip::udp::endpoint endpoint);
	virtual NetworkMessage* createNetworkMessage(char* data);
	virtual void handleMessage(NetworkMessage* message);
	virtual std::vector<ConnectionEndpoint> getConnectionEndpoints();
	
	std::map<
		unsigned int,
		std::pair<
			CellClient *,
			CellClient *
		>
	> createCellRequestContexts;
	
	unsigned int nextRequestId;

private:
	ClientNetworkManager(const ClientNetworkManager &other);

	udp::endpoint m_serverEndpoint;
	std::vector<ConnectionEndpoint> m_endpoints;
};

