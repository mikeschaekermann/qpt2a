#include "ClientNetworkManager.h"

#include "../../common/Config.h"

#include "../../common/network/messages/game/outgame/GameOver.h"
#include "../../common/network/messages/game/outgame/JoinFailure.h"
#include "../../common/network/messages/game/outgame/JoinSuccess.h"
#include "../../common/network/messages/game/outgame/StartGame.h"

#include "../../common/network/messages/game/ingame/cell/combat/CellAttack.h"
#include "../../common/network/messages/game/ingame/cell/combat/CellDie.h"

#include "../../common/network/messages/game/ingame/cell/creation/CellNew.h"
#include "../../common/network/messages/game/ingame/cell/creation/CreateCellComplete.h"
#include "../../common/network/messages/game/ingame/cell/creation/CreateCellRequest.h"
#include "../../common/network/messages/game/ingame/cell/creation/CreateCellFailure.h"
#include "../../common/network/messages/game/ingame/cell/creation/CreateCellSuccess.h"

#include "../../client/managers/GameManager.h"

#include "../../client/actors/CellClient.h"
#include "../../client/actors/StandardCellClient.h"

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
		memcpy(&messageType, &data[sizeof(unsigned)], sizeof(unsigned));
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
	switch (message->messageType.getType())
	{
	case MessageType::GameOver:
	{
		GameOver *gameOver = dynamic_cast<GameOver*> (message);
		if (gameOver)
		{
			
			LOG_INFO("GameOver received");
		}
		break;
	}
	case MessageType::JoinFailure:
	{
		JoinFailure *joinFailure= dynamic_cast<JoinFailure*> (message);
		if (joinFailure)
		{
			LOG_INFO("JoinFailure received");
		}
		break;
	}
	case MessageType::JoinSuccess:
	{
		JoinSuccess *joinSuccess = dynamic_cast<JoinSuccess*> (message);
		if (joinSuccess)
		{
			GAME_MGR->setMyPlayerId(joinSuccess->playerId);

			LOG_INFO("JoinSuccess received");
			LOG_INFO("====================");
			stringstream message;
			message << "my player id: " << joinSuccess->playerId;
			LOG_INFO(message.str());
		}
		break;
	}
	case MessageType::StartGame:
	{
		StartGame *startGame = dynamic_cast<StartGame*> (message);
		if (startGame)
		{
			stringstream message;

			LOG_INFO("StartGame received");
			LOG_INFO("==================");
			message << "world radius: " << startGame->worldRadius;
			LOG_INFO(message.str());
			message.str("");

			GAME_MGR->addPetriDish(startGame->worldRadius);

			for (auto it = startGame->players.begin(); it != startGame->players.end(); ++it)
			{
				message << "player " << it->playerName << "(" << it->playerId << ") with stem cell (" << it->startCellId << ") at position (" << it->startPosition.x << ", " << it->startPosition.y << ", " << it->startPosition.z << ")";
				LOG_INFO(message.str());
				message.str("");

				GAME_MGR->addPlayer(it->playerId, it->playerName, it->startCellId, it->startPosition);
			}
			SCREEN_MGR->openGameScreen();
		}
		break;
	}
	case MessageType::CellAttack:
	{
		CellAttack *cellAttack = dynamic_cast<CellAttack*> (message);
		if (cellAttack)
		{
			auto attacker = GAME_SCR.getGameObjectsToDraw().find(cellAttack->attackerCellId);
			assert(attacker != nullptr);
			dynamic_cast<StandardCellClient *>(attacker)->startAttackAnimation();

			auto attacked = GAME_SCR.getGameObjectsToDraw().find(cellAttack->attackedCellId);
			assert(attacked != nullptr);
			dynamic_cast<CellClient *>(attacked)->decreaseHealthPointsBy(cellAttack->damage);
			
			assert(attacker != nullptr && attacked != nullptr);
			ci::Vec3f cellVec = attacker->getPosition() - attacked->getPosition();
			cellVec.normalize();
			ci::Vec3f textPos = attacked->getPosition() + (cellVec * (attacker->getPosition() - attacked->getPosition()).length() / 2.f);
			GAME_SCR.addRenderText(
				GameScreen::RenderText(
					getElapsedSeconds() + CONFIG_FLOAT2("data.ingamefeedback.renderedDamage.displaytime", 5.f),
					textPos,
					stringify(ostringstream() << ceil(cellAttack->damage))));

			LOG_INFO("CellAttack received");
		}
		break;
	}
	case MessageType::CellDie:
	{
		CellDie *cellDie = dynamic_cast<CellDie*> (message);
		if (cellDie)
		{
			LOG_INFO("CellDie received");
		}
		break;
	}
	case MessageType::CellNew:
	{
		CellNew *cellNew = dynamic_cast<CellNew*> (message);
		if (cellNew)
		{
			LOG_INFO("CellNew received");

			GAME_SCR.addIncompleteCell(
				cellNew->playerId, 
				cellNew->type.getType(),
				cellNew->cellId,
				cellNew->position,
				cellNew->angle
			);
		}
		break;
	}
	case MessageType::CreateCellComplete:
	{
		CreateCellComplete *createCellComplete = dynamic_cast<CreateCellComplete*> (message);
		if (createCellComplete)
		{
			LOG_INFO("CreateCellComplete received");
			GAME_SCR.completeCellById(createCellComplete->cellId);
		}
		break;
	}
	case MessageType::CreateCellSuccess:
	{
		CreateCellSuccess *createCellSuccess = dynamic_cast<CreateCellSuccess*> (message);
		if (createCellSuccess)
		{
			LOG_INFO("CreateCellSuccess received");
			unsigned int requestId = createCellSuccess->requestId;

			auto context = createCellRequestContexts.find(requestId);

			if (context != createCellRequestContexts.end())
			{
				auto newCell = context->second.first;
				auto parentCell = context->second.second;

				newCell->setPosition(createCellSuccess->position);
				newCell->setAngle(createCellSuccess->angle);
				newCell->setId(createCellSuccess->cellId);
				newCell->addParent(parentCell);
				parentCell->addChild(newCell);
				GAME_SCR.removeCellPreview(newCell);
				GAME_SCR.addIncompleteCell(newCell);

				createCellRequestContexts.erase(context);
			}
		}
		break;
	}
	case MessageType::CreateCellFailure:
	{
		CreateCellFailure *createCellFailure = dynamic_cast<CreateCellFailure*> (message);
		if (createCellFailure)
		{
			LOG_INFO("CreateCellFailure received");
			unsigned int requestId = createCellFailure->requestId;

			auto context = createCellRequestContexts.find(requestId);

			if (context != createCellRequestContexts.end())
			{
				auto newCell = context->second.first;

				GAME_SCR.removeCellPreview(newCell);

				createCellRequestContexts.erase(context);
			}
		}
		break;
	}
	default:
	{
		LOG_WARNING("Message received that could not be categorized.");
		break;
	}
	}
}

vector<ConnectionEndpoint> ClientNetworkManager::getConnectionEndpoints()
{		
	return m_endpoints;
}

ClientNetworkManager::ClientNetworkManager(udp::endpoint serverEndpoint) :
	NetworkManager(),
	m_serverEndpoint(serverEndpoint),
	nextRequestId(0)
{
	m_endpoints.push_back(ConnectionEndpoint(serverEndpoint));
}

void ClientNetworkManager::registerCreateCellRequest(
	CreateCellRequest * request,
	CellClient * newCell,
	CellClient * parentCell
)
{
	createCellRequestContexts.insert(make_pair(
		nextRequestId,
		make_pair(
			newCell,
			parentCell
		)
	));

	request->requestId = nextRequestId;

	++nextRequestId;
}