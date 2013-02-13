#include "ServerNetworkManager.h"
#include "../game/GameContext.h"
#include "../game/Game.h"
#include "../game/PlayerServer.h"
#include "../../common/network/messages/game/outgame/JoinRequest.h"
#include "../../common/network/messages/game/ingame/cell/creation/CreateCellRequest.h"

using namespace std;

ConnectionEndpoint* ServerNetworkManager::getConnectionEndpoint(boost::asio::ip::udp::endpoint endpoint)
{
	for (auto it = GAMECONTEXT->getPlayerMap().begin(); it != GAMECONTEXT->getPlayerMap().end(); ++it)
	{
		if (it->second->getEndpoint() == endpoint)
		{
			return it->second;
		}
	}

	return 0;
}

NetworkMessage* ServerNetworkManager::createNetworkMessage(char* data)
{
	NetworkMessage *message = NetworkManager::createNetworkMessage(data);

	if (!message)
	{
		unsigned messageType;
		memcpy(&messageType, &data[sizeof(unsigned)], sizeof(unsigned));
		MessageType type(messageType);
		
		unsigned index = 0;
		switch (type.getType())
		{
		case MessageType::JoinRequest:
			{
				message = new JoinRequest(data, index);
				break;
			}
		case MessageType::CreateCellRequest:
			{
				message = new CreateCellRequest(data, index);
				break;
			}
		default:
			break;
		}
	}
	return message;
}

void ServerNetworkManager::handleMessage(NetworkMessage* message)
{
	GAMECONTEXT->getMutex().lock();
	JoinRequest *joinRequest = dynamic_cast<JoinRequest*> (message);
	if (joinRequest)
	{
		m_game->join(*joinRequest);
	}

	CreateCellRequest *createCellRequest = dynamic_cast<CreateCellRequest*> (message);
	if (createCellRequest)
	{
		m_game->createCell(*createCellRequest);
	}
	GAMECONTEXT->getMutex().unlock();
}

vector<ConnectionEndpoint> ServerNetworkManager::getConnectionEndpoints()
{
	vector<ConnectionEndpoint> endpoints;
	for (auto it = GAMECONTEXT->getPlayerMap().begin(); it != GAMECONTEXT->getPlayerMap().end(); ++it)
	{
		endpoints.push_back(*(it->second));
	}
		
	return endpoints;
}

ServerNetworkManager::ServerNetworkManager(unsigned short listenPort, Game *game) : NetworkManager(listenPort), m_game(game)
{
}