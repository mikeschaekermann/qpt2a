#pragma once

#include "../../common/network/NetworkManager.h"
#include <map>

using namespace boost::asio::ip;

class CreateCellRequest;
class CreateCellSuccess;
class CreateCellFailure;

class ClientNetworkManager : public NetworkManager
{
public:
	ClientNetworkManager(udp::endpoint serverEndpoint);
	
	virtual void registerCreateCellCallbacks(
		CreateCellRequest * request,
		std::function<void(CreateCellSuccess *)> successCallback = nullptr,
		std::function<void(CreateCellFailure *)> failureCallback = nullptr
	);
	
protected:
	virtual ConnectionEndpoint* getConnectionEndpoint(boost::asio::ip::udp::endpoint endpoint);
	virtual NetworkMessage* createNetworkMessage(char* data);
	virtual void handleMessage(NetworkMessage* message);
	virtual std::vector<ConnectionEndpoint> getConnectionEndpoints();
	
	std::map<
		unsigned int,
		std::pair<
			std::function<void(CreateCellSuccess *)>,
			std::function<void(CreateCellFailure *)>
		>
	> createCellCallbacks;
	
	unsigned int nextRequestId;

private:
	ClientNetworkManager(const ClientNetworkManager &other);

	udp::endpoint m_serverEndpoint;
	std::vector<ConnectionEndpoint> m_endpoints;
};

