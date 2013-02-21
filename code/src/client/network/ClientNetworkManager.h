#pragma once

#include "../../common/network/NetworkManager.h"
#include <map>
#include <set>
#include <tuple>

using namespace boost::asio::ip;

class CreateCellRequest;
class CreatePolypeptideRequest;
class MovePolypeptideRequest;
class CellClient;
class PolypeptideClient;
class JoinSuccess;
class StartGame;

class ClientNetworkManager : public NetworkManager
{
public:
	ClientNetworkManager(udp::endpoint serverEndpoint);
	
	virtual void registerCreateCellRequest(
		CreateCellRequest * request,
		CellClient * newCell,
		CellClient * parentCell
	);
	virtual void removeCreateCellRequestByParentCell(CellClient * parentCell);

	virtual void registerCreatePolypeptideRequest(CreatePolypeptideRequest * request);

	virtual void registerMovePolypeptideRequest(MovePolypeptideRequest * request, CellClient * fromCell, CellClient * toCell, unsigned int amount);
	
protected:
	virtual ConnectionEndpoint* getConnectionEndpoint(boost::asio::ip::udp::endpoint endpoint);
	virtual NetworkMessage* createNetworkMessage(char* data);
	virtual void handleMessage(NetworkMessage* message);
	virtual std::vector<ConnectionEndpoint> getConnectionEndpoints();
	
	std::map<unsigned int, std::pair<CellClient *,CellClient *> >
		createCellRequestContexts;
	std::set<unsigned int> createPolypeptideRequestContexts;
	std::map<unsigned int, std::tuple<CellClient *, CellClient *, unsigned int> > movePolypeptideRequestContexts;
	
	unsigned int nextRequestId;

private:
	ClientNetworkManager(const ClientNetworkManager &other);

	void initiateGame();

	udp::endpoint m_serverEndpoint;
	std::vector<ConnectionEndpoint> m_endpoints;

	JoinSuccess * joinSuccessBuffered;
	StartGame * startGameBuffered;
};

