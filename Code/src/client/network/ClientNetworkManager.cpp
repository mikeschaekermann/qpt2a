#include "ClientNetworkManager.h"

#include "../../common/network/messages/game/outgame/GameOver.h"
#include "../../common/network/messages/game/outgame/JoinFailure.h"
#include "../../common/network/messages/game/outgame/JoinSuccess.h"
#include "../../common/network/messages/game/outgame/StartGame.h"

#include "../../common/network/messages/game/ingame/cell/combat/CellAttack.h"
#include "../../common/network/messages/game/ingame/cell/combat/CellDie.h"

#include "../../common/network/messages/game/ingame/cell/creation/CellNew.h"
#include "../../common/network/messages/game/ingame/cell/creation/CreateCellComplete.h"
#include "../../common/network/messages/game/ingame/cell/creation/CreateCellFailure.h"
#include "../../common/network/messages/game/ingame/cell/creation/CreateCellSuccess.h"

using namespace std;

ConnectionEndpoint* ClientNetworkManager::getConnectionEndpoint(boost::asio::ip::udp::endpoint endpoint)
{
	if (!m_endpoints.empty())
	{
		return &m_endpoints[0];
	}

	return 0;
}

NetworkMessage* ClientNetworkManager::createNetworkMessage(char* data)
{
	NetworkMessage *message = NetworkManager::createNetworkMessage(data);

	if (!message)
	{
		unsigned messageType;
		memcpy(&messageType, (void*) data[sizeof(unsigned)], sizeof(unsigned));
		MessageType type(messageType);
		
		unsigned index = 0;
		switch (type.getType())
		{
		case MessageType::GameOver:
			{
				message = new GameOver(data, index);
				break;
			}
		case MessageType::JoinFailure:
			{
				message = new JoinFailure(data, index);
				break;
			}
		case MessageType::JoinSuccess:
			{
				message = new JoinSuccess(data, index);
				break;
			}
		case MessageType::StartGame:
			{
				message = new StartGame(data, index);
				break;
			}
		case MessageType::CellAttack:
			{
				message = new CellAttack(data, index);
				break;
			}
		case MessageType::CellDie:
			{
				message = new CellDie(data, index);
				break;
			}
		case MessageType::CellNew:
			{
				message = new CellNew(data, index);
				break;
			}
		case MessageType::CreateCellComplete:
			{
				message = new CreateCellComplete(data, index);
				break;
			}
		case MessageType::CreateCellSuccess:
			{
				message = new CreateCellSuccess(data, index);
				break;
			}
		case MessageType::CreateCellFailure:
			{
				message = new CreateCellFailure(data, index);
				break;
			}

		default:
			break;
		}
	}
	return message;
}

void ClientNetworkManager::handleMessage(NetworkMessage* message)
{
	GameOver *gameOver = dynamic_cast<GameOver*> (message);
	if (gameOver)
	{
		/// TODO
	}

	JoinFailure *joinFailure = dynamic_cast<JoinFailure*> (message);
	if (joinFailure)
	{
		/// TODO
	}

	JoinSuccess *joinSuccess = dynamic_cast<JoinSuccess*> (message);
	if (joinSuccess)
	{
		/// TODO
	}

	StartGame *startGame = dynamic_cast<StartGame*> (message);
	if (startGame)
	{
		/// TODO
	}

	CellAttack *cellAttack = dynamic_cast<CellAttack*> (message);
	if (cellAttack)
	{
		/// TODO
	}

	CellDie *cellDie = dynamic_cast<CellDie*> (message);
	if (cellDie)
	{
		/// TODO
	}

	CellNew *cellNew = dynamic_cast<CellNew*> (message);
	if (cellNew)
	{
		/// TODO
	}
		
	CreateCellComplete *createCellComplete = dynamic_cast<CreateCellComplete*> (message);
	if (createCellComplete)
	{
		/// TODO
	}

	CreateCellSuccess *createCellSuccess = dynamic_cast<CreateCellSuccess*> (message);
	if (createCellSuccess)
	{
		/// TODO
	}

	CreateCellFailure *createCellFailure = dynamic_cast<CreateCellFailure*> (message);
	if (createCellFailure)
	{
		/// TODO
	}
}

vector<ConnectionEndpoint> ClientNetworkManager::getConnectionEndpoints()
{		
	return m_endpoints;
}

ClientNetworkManager::ClientNetworkManager(udp::endpoint serverEndpoint) : NetworkManager(), m_serverEndpoint(serverEndpoint)
{
	m_endpoints.push_back(ConnectionEndpoint(serverEndpoint));
}