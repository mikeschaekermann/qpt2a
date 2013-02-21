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
#include "../screens/GameScreenStates/GameScreenStateNeutral.h"
#include "../screens/ConnectScreen.h"
#include "../screens/GameScreen.h"

#include "../../common/PolypeptideCapacityContainer.h"

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
		case MessageType::MovePolypeptideSuccess:
			{
				message = new MovePolypeptideSuccess(data, index);
				break;
			}
		case MessageType::MovePolypeptideFailure:
			{
				message = new MovePolypeptideFailure(data, index);
				break;
			}
		case MessageType::PolypeptideCellAttack:
			{
				message = new PolypeptideCellAttack(data, index);
				break;
			}
		case MessageType::PolypeptideFight:
			{
				message = new PolypeptideFight(data, index);
				break;
			}
		case MessageType::PolypeptideDie:
			{
				message = new PolypeptideDie(data, index);
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
	GAME_SCR->getContainerMutex().lock();

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
				GAME_SCR->switchToState(new GameScreenStateGameOver(GAME_SCR));
			}
		}
		break;
	}
	case MessageType::JoinFailure:
	{
		JoinFailure *joinFailure= dynamic_cast<JoinFailure*> (message);
		if (joinFailure)
		{
			CONN_SCR->joinFailure(joinFailure->errorCode);
			LOG_INFO("JoinFailure received");
		}
		break;
	}
	case MessageType::JoinSuccess:
	{
		JoinSuccess *joinSuccess = dynamic_cast<JoinSuccess*> (message);
		if (joinSuccess)
		{
			joinSuccessBuffered = new JoinSuccess();
			joinSuccessBuffered->playerId = joinSuccess->playerId;

			LOG_INFO("JoinSuccess received");
			LOG_INFO("====================");
			stringstream message;
			message << "my player id: " << joinSuccess->playerId;
			LOG_INFO(message.str());

			initiateGame();
		}
		break;
	}
	case MessageType::StartGame:
	{
		StartGame *startGame = dynamic_cast<StartGame*> (message);
		if (startGame)
		{
			startGameBuffered = new StartGame();
			startGameBuffered->barriers = startGame->barriers;
			startGameBuffered->dynamicModifiers = startGame->dynamicModifiers;
			startGameBuffered->players = startGame->players;
			startGameBuffered->staticModifiers = startGame->staticModifiers;
			startGameBuffered->worldRadius = startGame->worldRadius;

			stringstream message;

			LOG_INFO("StartGame received");
			LOG_INFO("==================");
			message << "world radius: " << startGame->worldRadius;
			LOG_INFO(message.str());
			message.str("");

			initiateGame();
		}
		break;
	}
	case MessageType::CellAttack:
	{
		CellAttack *cellAttack = dynamic_cast<CellAttack*> (message);
		if (cellAttack)
		{
			LOG_INFO("CellAttack received");
			auto attacker = GAME_SCR->getGameObjectsToDraw().find(cellAttack->attackerCellId);
			auto attacked = GAME_SCR->getGameObjectsToDraw().find(cellAttack->attackedCellId);

			if (attacker) dynamic_cast<StandardCellClient *>(attacker)->startAttackAnimation();
			if (attacked) dynamic_cast<CellClient *>(attacked)->decreaseHealthPointsBy(cellAttack->damage);

			if (attacker && attacked)
			{
				LOG_INFO("CellAttack: Both available");
				ci::Vec3f cellVec = attacker->getPosition() - attacked->getPosition();
				cellVec.normalize();
				ci::Vec3f textPos = attacked->getPosition() + (cellVec * (attacker->getPosition() - attacked->getPosition()).length() / 3.f);
				
				float deathTime = (float) getElapsedSeconds() + CONFIG_FLOAT("data.ingamefeedback.renderedDamage.displaytime");
				string text = stringify(ostringstream() << "-" << ceil((float) cellAttack->damage));
				GameScreen::RenderText renderText(deathTime, textPos, text);
				
				GAME_SCR->addRenderText(renderText);
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
			auto cellObject = GAME_SCR->getGameObjectsToDraw().find(cellDie->cellId);
			if (cellObject != nullptr)
			{
				GAME_SCR->removeGameObjectToCollide(cellObject);
				auto cellClient = dynamic_cast<CellClient *>(cellObject);

				if (cellClient != nullptr)
				{
					GAME_SCR->removeCellToPick(cellClient);
					removeCreateCellRequestByParentCell(cellClient);

					if (cellClient->getOwner() == GAME_MGR->getMyPlayer())
					{
						GAME_SCR->removeExploringCell(cellClient);
						cellClient->decreaseGlobalTypeCounter();
					}
				}
				GAME_SCR->removeGameObjectToUpdate(cellObject);
				GAME_SCR->removeGameObjectToDraw(cellObject);

				delete cellObject;
			}
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

			if (cellNew->playerId != GAME_MGR->getMyPlayer()->getId())
			{
				GAME_SCR->addIncompleteCell(
					cellNew->playerId, 
					cellNew->type.getType(),
					cellNew->cellId,
					cellNew->position,
					cellNew->angle
				);
			}
		}
		break;
	}
	case MessageType::CreateCellComplete:
	{
		CreateCellComplete *createCellComplete = dynamic_cast<CreateCellComplete*> (message);
		if (createCellComplete)
		{
			LOG_INFO("CreateCellComplete received");
			GAME_SCR->completeCellById(createCellComplete->cellId);
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
				newCell->setOpacity(CONFIG_FLOAT("data.ingamefeedback.building.incompleteOpacity"));
				newCell->setHue(GAME_MGR->getMyHue());
				newCell->increaseGlobalTypeCounter();

				parentCell->addChild(newCell);
				/// must be hidden so that the skin is updated later!
				newCell->hide();
				GAME_SCR->removeCellPreview(newCell);
				GAME_SCR->addIncompleteCell(newCell);
				GAME_SCR->addExploringCell(newCell);
				
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

				GAME_SCR->removeCellPreview(newCell);
				GAME_SCR->switchToState(new GameScreenStateCreateCell(GAME_SCR, parentCell, newCell));
				
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
			unsigned int polypeptideId = createPolySuccess->polypeptideId;

			auto context = createPolypeptideRequestContexts.find(requestId);

			if (context != createPolypeptideRequestContexts.end())
			{
				CellClient & stemCell = GAME_MGR->getMyPlayer()->getStemCell();

				PolypeptideClient * polypeptide = new PolypeptideClient(stemCell.getPosition(), 10, stemCell.getRadius());
				polypeptide->setId(polypeptideId);
				polypeptide->setPosition(stemCell.getPosition());
				polypeptide->show();
				polypeptide->setOwner(&stemCell);

				++(POLYCAPACITY->NumberOfPolypeptides);

				stemCell.addPolypeptide(polypeptide);

				GAME_SCR->getMyPolypeptides().addGameObject(polypeptide);
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
		break;
	}
	case MessageType::MovePolypeptideSuccess:
	{
		MovePolypeptideSuccess * movePolypeptideSuccess = dynamic_cast<MovePolypeptideSuccess *>(message);
		if (movePolypeptideSuccess)
		{
			LOG_INFO("MovePolypeptideSuccess received");
			unsigned int requestId = movePolypeptideSuccess->requestId;
			auto polypeptideIds = movePolypeptideSuccess->polypeptideIds;

			auto context = movePolypeptideRequestContexts.find(requestId);

			if (context != movePolypeptideRequestContexts.end())
			{
				auto fromCell = std::get<0>(context->second);
				auto toCell = std::get<1>(context->second);
				auto amount = std::get<2>(context->second);
				/// make travel

				if (polypeptideIds.size() <= amount)
				{
					for (auto it = polypeptideIds.begin(); it != polypeptideIds.end(); ++it)
					{
						Polypeptide * polypeptide = fromCell->getPolypeptides().find(*it)->second;
						toCell->addPolypeptide(polypeptide);
						fromCell->removePolypeptide(polypeptide);

						polypeptide->setOwner(toCell);
						auto polypeptideClient = dynamic_cast<PolypeptideClient *>(polypeptide);
						if (polypeptideClient != nullptr)
						{
							polypeptideClient->setFocus(toCell->getPosition(), toCell->getRadius());
							polypeptideClient->setState(Polypeptide::IDLE);
						}
						
						auto polypeptideInSelectionList = GAME_SCR->getSelectedPolypeptides().find(*it);
						if (polypeptideInSelectionList != nullptr)
						{
							GAME_SCR->getSelectedPolypeptides().removeGameObject(polypeptideInSelectionList);
						}
						else
						{
							LOG_ERROR("Tried to move polypeptide which the client does not have in its list!");
							assert(false);
						}
					}
				}

				if (GAME_SCR->getSelectedPolypeptides().getSize() == 0)
				{
					GAME_SCR->switchToState(new GameScreenStateNeutral(GAME_SCR));
				}

				movePolypeptideRequestContexts.erase(context);
			}
		}
		break;
	}
	case MessageType::MovePolypeptideFailure:
	{
		MovePolypeptideFailure * movePolypeptideFailure = dynamic_cast<MovePolypeptideFailure *>(message);
		if (movePolypeptideFailure)
		{
			LOG_INFO("MovePolypeptideFailure received");
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
		break;
	}
	case MessageType::PolypeptideDie:
	{
		PolypeptideDie * polypeptideDie = dynamic_cast<PolypeptideDie *>(message);
		if (polypeptideDie)
		{
			LOG_INFO("PolypeptideDie received");
			
			unsigned int polypeptideId = polypeptideDie->polypeptideId;

			auto polypeptide = GAME_SCR->getMyPolypeptides().find(polypeptideId);
			if (polypeptide != nullptr)
			{
				polypeptide->getOwner()->removePolypeptide(polypeptide);
				GAME_SCR->getSelectedPolypeptides().removeGameObject(polypeptide);
				GAME_SCR->getMyPolypeptides().removeGameObject(polypeptide);

				if (POLYCAPACITY->NumberOfPolypeptides == 0)
				{
					LOG_ERROR("Tried to remove a polypeptide although the polypeptide counter is already zero.");
					assert(false);
				}
				else
				{
					--(POLYCAPACITY->NumberOfPolypeptides);
				}

				delete polypeptide;
			}
			else
			{
				LOG_ERROR("Tried to delete polypeptide which the client does not have in its list!");
				assert(false);
			}
		}
		break;
	}
	case MessageType::PolypeptideCellAttack:
	{
		PolypeptideCellAttack * polypeptideCellAttack = dynamic_cast<PolypeptideCellAttack *>(message);
		if (polypeptideCellAttack)
		{
			LOG_INFO("PolypeptideCellAttack received");
			unsigned int polypeptideId = polypeptideCellAttack->polypeptideId;
			unsigned int attackedCellId = polypeptideCellAttack->attackedCellId;
			unsigned int attackerCellId = polypeptideCellAttack->attackerCellId;
			float damage = polypeptideCellAttack->damage;

			auto attackedObject = GAME_SCR->getGameObjectsToDraw().find(attackedCellId);
			if (attackedObject == nullptr)
			{
				LOG_ERROR("Attacked object does not exist!");
				assert(false);
			}

			auto attackerObject = GAME_SCR->getGameObjectsToDraw().find(attackerCellId);
			if (attackerObject == nullptr)
			{
				LOG_ERROR("Attacker object does not exist!");
				assert(false);
			}

			auto attackedCell = dynamic_cast<CellClient *>(attackedObject);
			if (attackedCell == nullptr)
			{
				LOG_ERROR("Attacked object is not of type CellClient!");
				assert(false);
			}

			auto attackerCell = dynamic_cast<CellClient *>(attackerObject);
			if (attackerCell == nullptr)
			{
				LOG_ERROR("Attacker object is not of type CellClient!");
				assert(false);
			}

			bool ownCell = attackedCell->getOwner() == GAME_MGR->getMyPlayer();
				
			auto polypeptide = GAME_SCR->getMyPolypeptides().find(polypeptideId);
			bool ownPoly = polypeptide != nullptr;

			/// something terribly wrong is happening here:
			/// a) my own poly is attacking me
			/// b) I am getting a message that does not affect me at all!
			if (ownCell == ownPoly)
			{
				LOG_ERROR("Either the attacking poly or the cell being attacked must be mine and the other one must not be mine!");
				assert(false);
			}

			/// other player's poly is attacking!
			if (!ownPoly)
			{
				polypeptide = new PolypeptideClient();
				polypeptide->setId(polypeptideId);
				polypeptide->setPosition(attackerCell->getPosition());
				polypeptide->setAttackOptions(attackerCell->getPosition(), true, false);
				polypeptide->setOwner(attackerCell);
				GAME_SCR->addGameObjectToDraw(polypeptide);
			}
			else
			{
				polypeptide->setAttackOptions(attackerCell->getPosition(), false, false);
			}

			polypeptide->setFocus(attackedCell->getPosition(), attackerCell->getRadius());
			attackedCell->decreaseHealthPointsBy(damage);
			polypeptide->setState(Polypeptide::CELLFIGHT);
		}
		break;
	}
	case MessageType::PolypeptideFight:
	{
		PolypeptideFight * polypeptideFight = dynamic_cast<PolypeptideFight *>(message);
		if (polypeptideFight)
		{
			LOG_INFO("PolypeptideFight received");

			unsigned int cellId1 = polypeptideFight->cell1Id;
			unsigned int cellId2 = polypeptideFight->cell2Id;
			unsigned int polypeptideId1 = polypeptideFight->polypeptideId1;
			unsigned int polypeptideId2 = polypeptideFight->polypeptideId2;
			bool polypeptide1Dies = polypeptideFight->polypeptide1Dies;
			bool polypeptide2Dies = polypeptideFight->polypeptide2Dies;

			auto cellObject1 = GAME_SCR->getGameObjectsToDraw().find(cellId1);
			if (cellObject1 == nullptr)
			{
				LOG_ERROR("Parent cell of poly #1 could not be found!");
				assert(false);
			}

			auto cellObject2 = GAME_SCR->getGameObjectsToDraw().find(cellId2);
			if (cellObject2 == nullptr)
			{
				LOG_ERROR("Parent cell of poly #2 could not be found!");
				assert(false);
			}

			auto cellClient1 = dynamic_cast<CellClient *>(cellObject1);
			if (cellClient1 == nullptr)
			{
				LOG_ERROR("Parent cell of poly #1 is not of type CellClient!");
				assert(false);
			}

			auto cellClient2 = dynamic_cast<CellClient *>(cellObject2);
			if (cellClient2 == nullptr)
			{
				LOG_ERROR("Parent cell of poly #2 is not of type CellClient!");
				assert(false);
			}

			auto polyObject1 = GAME_SCR->getMyPolypeptides().find(polypeptideId1);
			if (cellObject1 == nullptr)
			{
				polyObject1 = new PolypeptideClient();
				polyObject1->setId(polypeptideId1);
				polyObject1->setPosition(cellClient1->getPosition());
				polyObject1->setAttackOptions(cellClient1->getPosition(), true, polypeptide1Dies);
				polyObject1->setOwner(cellClient1);
				GAME_SCR->addGameObjectToDraw(polyObject1);
			}
			else
			{
				polyObject1->setAttackOptions(cellClient1->getPosition(), false, polypeptide1Dies);
			}

			auto polyObject2 = GAME_SCR->getMyPolypeptides().find(polypeptideId2);
			if (cellObject2 == nullptr)
			{
				polyObject2 = new PolypeptideClient();
				polyObject2->setId(polypeptideId2);
				polyObject2->setPosition(cellClient2->getPosition());
				polyObject2->setAttackOptions(cellClient2->getPosition(), true, polypeptide2Dies);
				polyObject2->setOwner(cellClient2);
				GAME_SCR->addGameObjectToDraw(polyObject2);
			}
			else
			{
				polyObject2->setAttackOptions(cellClient2->getPosition(), false, polypeptide2Dies);
			}

			auto focusCenter = (polyObject1->getPosition() + polyObject2->getPosition()) * 0.5f;

			polyObject1->setFocus(focusCenter, cellClient1->getRadius());
			polyObject1->setState(Polypeptide::POLYPEPTIDEFIGHT);

			polyObject2->setFocus(focusCenter, cellClient2->getRadius());
			polyObject2->setState(Polypeptide::POLYPEPTIDEFIGHT);
		}
		break;
	}
	default:
	{
		LOG_WARNING("Message received that could not be categorized.");
		break;
	}
	}

	GAME_SCR->getContainerMutex().unlock();
}

vector<ConnectionEndpoint> ClientNetworkManager::getConnectionEndpoints()
{		
	return m_endpoints;
}

ClientNetworkManager::ClientNetworkManager(udp::endpoint serverEndpoint) :
	NetworkManager(),
	m_serverEndpoint(serverEndpoint),
	nextRequestId(0),
	joinSuccessBuffered(nullptr),
	startGameBuffered(nullptr)
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

void ClientNetworkManager::initiateGame()
{
	if (joinSuccessBuffered != nullptr && startGameBuffered != nullptr)
	{
		auto joinSuccess = joinSuccessBuffered;
		auto startGame = startGameBuffered;

		/// handle join success message
		GAME_MGR->setMyPlayerId(joinSuccess->playerId);
		CONN_SCR->joinSuccess();

		/// handle start game message
		stringstream message;

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

		/// delete buffered messages
		delete joinSuccessBuffered;
		joinSuccessBuffered = nullptr;

		delete startGameBuffered;
		startGameBuffered = nullptr;
	}
}