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

#include "../../common/network/messages/game/ingame/polypeptide/creation/CreatePolypeptideRequest.h"
#include "../../common/network/messages/game/ingame/polypeptide/creation/CreatePolypeptideSuccess.h"
#include "../../common/network/messages/game/ingame/polypeptide/creation/CreatePolypeptideFailure.h"

#include "../../common/network/messages/game/ingame/polypeptide/combat/PolypeptideFight.h"
#include "../../common/network/messages/game/ingame/polypeptide/combat/PolypeptideCellAttack.h"
#include "../../common/network/messages/game/ingame/polypeptide/combat/PolypeptideDie.h"

#include "../../common/network/messages/game/ingame/polypeptide/travel/MovePolypeptideRequest.h"
#include "../../common/network/messages/game/ingame/polypeptide/travel/MovePolypeptideSuccess.h"
#include "../../common/network/messages/game/ingame/polypeptide/travel/MovePolypeptideFailure.h"

#include "../../client/managers/GameManager.h"

#include "../../client/actors/CellClient.h"
#include "../../client/actors/StandardCellClient.h"
#include "../../client/actors/PolypeptideClient.h"

#include "../screens/GameScreenStates/GameScreenStateGameOver.h"
#include "../screens/GameScreenStates/GameScreenStateCreateCell.h"
#include "../screens/ConnectScreen.h"

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
		case MessageType::CreatePolypeptideSuccess:
			{
				message = new CreatePolypeptideSuccess(data, index);
				break;
			}
		case MessageType::CreatePolypeptideFailure:
			{
				message = new CreatePolypeptideFailure(data, index);
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
	GAME_SCR.getContainerMutex().lock();

	switch (message->messageType.getType())
	{
	case MessageType::GameOver:
	{
		GameOver *gameOver = dynamic_cast<GameOver*> (message);
		if (gameOver)
		{
			if (gameOver->playerId == GAME_MGR->getMyPlayer()->getId())
			{
				SOUND_PLAYER->playSound(string("gameOver"));
				GAME_SCR.switchToState(new GameScreenStateGameOver(&GAME_SCR));
			}
		}
		break;
	}
	case MessageType::JoinFailure:
	{
		JoinFailure *joinFailure= dynamic_cast<JoinFailure*> (message);
		if (joinFailure)
		{
			CONN_SCR.joinFailure(joinFailure->errorCode);
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
			CONN_SCR.joinSuccess();
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

			// Create world
			GAME_MGR->addPetriDish(startGame->worldRadius);

			// Add all players
			for (auto it = startGame->players.begin(); it != startGame->players.end(); ++it)
			{
				message << "player " << it->playerName << "(" << it->playerId << ") with stem cell (" << it->startCellId << ") at position (" << it->startPosition.x << ", " << it->startPosition.y << ", " << it->startPosition.z << ")";
				LOG_INFO(message.str());
				message.str("");

				GAME_MGR->addPlayer(it->playerId, it->playerName, it->startCellId, it->startPosition);
			}
			
			// Add barriers
			for (auto it = startGame->barriers.begin(); it != startGame->barriers.end(); ++it)
			{
				message << "barrier " << it->modifierId << endl;
				LOG_INFO(message.str());
				message.str("");

				GAME_MGR->addBarrier(it->modifierId, it->position, it->rotation, it->scale, it->radius);
			}

			// Add static environment
			for (auto it = startGame->staticModifiers.begin(); it != startGame->staticModifiers.end(); ++it)
			{
				message << "static " << it->modifierId << it->type.getType() << endl;
				LOG_INFO(message.str());
				message.str("");
				StaticModificator::Type type = StaticModificator::NUTRIENTSOIL;

				switch (it->type.getType())
				{
				case StaticModifierType::NutrientSoil:
					type = StaticModificator::NUTRIENTSOIL;
					break;
				case StaticModifierType::RadioActivity:
					type = StaticModificator::RADIOACTIVITY;
					break;
				default:
					continue;
				}
				GAME_MGR->addStaticModifier(it->modifierId, it->position, it->rotation, it->scale, it->radius, type);
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
			LOG_INFO("CellAttack received");
			auto attacker = GAME_SCR.getGameObjectsToDraw().find(cellAttack->attackerCellId);
			auto attacked = GAME_SCR.getGameObjectsToDraw().find(cellAttack->attackedCellId);

			if (attacker) dynamic_cast<StandardCellClient *>(attacker)->startAttackAnimation();
			if (attacked) dynamic_cast<CellClient *>(attacked)->decreaseHealthPointsBy(cellAttack->damage);

			SOUND_PLAYER->playSound(string("cellAttack"));
			if (attacker && attacked)
			{
				LOG_INFO("CellAttack: Both available");
				ci::Vec3f cellVec = attacker->getPosition() - attacked->getPosition();
				cellVec.normalize();
				ci::Vec3f textPos = attacked->getPosition() + (cellVec * (attacker->getPosition() - attacked->getPosition()).length() / 3.f);
				
				float deathTime = (float) getElapsedSeconds() + CONFIG_FLOAT2("data.ingamefeedback.renderedDamage.displaytime", 5.f);
				string text = stringify(ostringstream() << "-" << ceil((float) cellAttack->damage));
				GameScreen::RenderText renderText(deathTime, textPos, text);
				
				GAME_SCR.addRenderText(renderText);
			}
			LOG_INFO(stringify(ostringstream() << "Cell with id: " << cellAttack->attackerCellId << " is attacking cell with id: " << cellAttack->attackedCellId << " width a damage of: " << cellAttack->damage));
			
			LOG_INFO("CellAttack finished");
		}
		break;
	}
	case MessageType::CellDie:
	{
		CellDie *cellDie = dynamic_cast<CellDie*> (message);
		if (cellDie)
		{
			SOUND_PLAYER->playSound(string("cellDie"));
			auto cellObject = GAME_SCR.getGameObjectsToDraw().find(cellDie->cellId);
			if (cellObject != nullptr)
			{
				GAME_SCR.removeGameObjectToCollide(cellObject);
				auto cellClient = dynamic_cast<CellClient *>(cellObject);

				if (cellClient != nullptr)
				{
					GAME_SCR.removeCellToPick(cellClient);
					removeCreateCellRequestByParentCell(cellClient);

					if (cellClient->getOwner() == GAME_MGR->getMyPlayer())
					{
						GAME_SCR.removeExploringCell(cellClient);
					}
				}
				GAME_SCR.removeGameObjectToUpdate(cellObject);
				GAME_SCR.removeGameObjectToDraw(cellObject);

				delete cellObject;
			}
			LOG_INFO("CellDie received");
		}
		break;
	}
	case MessageType::CellNew:
	{
		CellNew *cellNew = dynamic_cast<CellNew*> (message);
		if (cellNew && cellNew->playerId != GAME_MGR->getMyPlayer()->getId())
		{
			LOG_INFO("CellNew received");

			GAME_SCR.addIncompleteCell(
				cellNew->playerId, 
				cellNew->type.getType(),
				cellNew->cellId,
				cellNew->position,
				cellNew->angle
			);

			LOG_INFO("cell angle raw (should be radians):");
			LOG_INFO(cellNew->angle);
			LOG_INFO("cell angle degrees:");
			LOG_INFO(ci::toDegrees(cellNew->angle));
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
				newCell->setOpacity(CONFIG_FLOAT2("data.ingamefeedback.building.incompleteOpacity", 0.5f));
				newCell->setHue(GAME_MGR->getMyHue());
				parentCell->addChild(newCell);
				GAME_SCR.removeCellPreview(newCell);
				GAME_SCR.addIncompleteCell(newCell);
				GAME_SCR.addExploringCell(newCell);
				
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
				auto parentCell = context->second.second;

				GAME_SCR.removeCellPreview(newCell);
				GAME_SCR.switchToState(new GameScreenStateCreateCell(&GAME_SCR, parentCell, newCell));
				
				createCellRequestContexts.erase(context);
			}
		}
		break;
	}
	case MessageType::CreatePolypeptideSuccess:
	{
		CreatePolypeptideSuccess * createPolySuccess = dynamic_cast<CreatePolypeptideSuccess *>(message);
		if (createPolySuccess)
		{
			LOG_INFO("CreatePolipeptideSuccess received");
			unsigned int requestId = createPolySuccess->requestId;

			auto context = createPolypeptideRequestContexts.find(requestId);

			if (context != createPolypeptideRequestContexts.end())
			{
				/// set values
				/// add to list
				createPolypeptideRequestContexts.erase(context);
			}
		}
		break;
	}
	case MessageType::CreatePolypeptideFailure:
	{
		CreatePolypeptideFailure * createPolyFailure = dynamic_cast<CreatePolypeptideFailure *>(message);
		if (createPolyFailure)
		{
			LOG_INFO("CreatePolipeptideFailure received");
			unsigned int requestId = createPolyFailure->requestId;

			auto context = createPolypeptideRequestContexts.find(requestId);

			if (context != createPolypeptideRequestContexts.end())
			{

				/// delete polypeptide

				createPolypeptideRequestContexts.erase(context);
			}
		}
	}
	case MessageType::MovePolypeptideSuccess:
	{
		MovePolypeptideSuccess * movePolypeptideSuccess = dynamic_cast<MovePolypeptideSuccess *>(message);
		if (movePolypeptideSuccess)
		{
			LOG_INFO("MovePolipeptideSuccess received");
			unsigned int requestId = movePolypeptideSuccess->requestId;

			auto context = movePolypeptideRequestContexts.find(requestId);

			if (context != movePolypeptideRequestContexts.end())
			{
				auto fromCell = std::get<0>(context->second);
				auto toCell = std::get<1>(context->second);
				auto amount = std::get<2>(context->second);
				/// make travel

				movePolypeptideRequestContexts.erase(context);
			}
		}
	}
	case MessageType::MovePolypeptideFailure:
	{
		MovePolypeptideFailure * movePolypeptideFailure = dynamic_cast<MovePolypeptideFailure *>(message);
		if (movePolypeptideFailure)
		{
			LOG_INFO("MovePolipeptideFailure received");
			unsigned int requestId = movePolypeptideFailure->requestId;

			auto context = movePolypeptideRequestContexts.find(requestId);

			if (context != movePolypeptideRequestContexts.end())
			{
				auto fromCell = std::get<0>(context->second);
				auto toCell = std::get<1>(context->second);
				auto amount = std::get<2>(context->second);
				/// error reaction

				movePolypeptideRequestContexts.erase(context);
			}
		}
	}
	case MessageType::PolypeptideDie:
	{
		PolypeptideDie * polypeptideDie = dynamic_cast<PolypeptideDie *>(message);
		if (polypeptideDie)
		{
			LOG_INFO("PolipeptideDie received");
			unsigned int polypeptideId = polypeptideDie->polypeptideId;
		}
	}
	case MessageType::PolypeptideCellAttack:
	{
		PolypeptideCellAttack * polypeptideCellAttack = dynamic_cast<PolypeptideCellAttack *>(message);
		if (polypeptideCellAttack)
		{
			LOG_INFO("PolipeptideCellAttack received");
			unsigned int polypeptideId = polypeptideCellAttack->polypeptideId;
			unsigned int cellId = polypeptideCellAttack->cellId;
			float damage = polypeptideCellAttack->damage;
		}
	}
	case MessageType::PolypeptideFight:
	{
		PolypeptideFight * polypeptideFight = dynamic_cast<PolypeptideFight *>(message);
		if (polypeptideFight)
		{
			LOG_INFO("PolipeptideFight received");
			unsigned int polypeptideId1 = polypeptideFight->polypeptideId1;
			unsigned int polypeptideId2 = polypeptideFight->polypeptideId2;
			bool polypeptide1Dies = polypeptideFight->polypeptide1Dies;
			bool polypeptide2Dies = polypeptideFight->polypeptide2Dies;
		}
	}
	default:
	{
		LOG_WARNING("Message received that could not be categorized.");
		break;
	}
	}

	GAME_SCR.getContainerMutex().unlock();
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

void ClientNetworkManager::removeCreateCellRequestByParentCell(CellClient * parentCell)
{
	for (auto it = createCellRequestContexts.begin(); it != createCellRequestContexts.end();)
	{
		if (it->second.second == parentCell)
		{
			it = createCellRequestContexts.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void ClientNetworkManager::registerCreatePolypeptideRequest(CreatePolypeptideRequest * request)
{
	createPolypeptideRequestContexts.insert(nextRequestId);
	request->requestId = nextRequestId;
	++nextRequestId;
}

void ClientNetworkManager::registerMovePolypeptideRequest(MovePolypeptideRequest * request, CellClient * fromCell, CellClient * toCell, unsigned int amount)
{
	movePolypeptideRequestContexts.insert(make_pair(nextRequestId, make_tuple(fromCell, toCell, amount)));
	request->requestId = nextRequestId;
	++nextRequestId;
}