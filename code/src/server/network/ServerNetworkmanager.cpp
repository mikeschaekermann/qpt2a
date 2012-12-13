#include "ServerNetworkManager.h"
#include "../game/Game.h"
#include "../../common/network/messages/game/outgame/JoinRequest.h"
#include "../../common/network/messages/game/ingame/cell/creation/CreateCellRequest.h"

using namespace std;

ConnectionEndpoint* ServerNetworkManager::getConnectionEndpoint(boost::asio::ip::udp::endpoint endpoint)
{
	for (std::vector<Player*>::iterator it = m_game->m_players.begin(); it != m_game->m_players.end(); ++it)
	{
		if ((*it)->getEndpoint() == endpoint)
		{
			return *it;
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
}

vector<ConnectionEndpoint> ServerNetworkManager::getConnectionEndpoints()
{
	vector<ConnectionEndpoint> endpoints;
	for (unsigned i = 0; i < m_game->m_players.size(); ++i)
	{
		endpoints.push_back(*m_game->m_players[i]);
	}
		
	return endpoints;
}

ServerNetworkManager::ServerNetworkManager(unsigned short listenPort, Game *game) : NetworkManager(listenPort), m_game(game)
{
}