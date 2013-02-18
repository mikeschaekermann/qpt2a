#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif

#include "Game.h"

#include <string>

#include <vector>
#include <cmath>
#include <sstream>

#include "boost/asio.hpp"

#include "../../common/network/NetworkManager.h"
#include "../network/ServerNetworkManager.h"
#include "../../common/network/messages/game/outgame/JoinRequest.h"
#include "../../common/network/messages/game/outgame/JoinFailure.h"
#include "../../common/network/messages/game/outgame/JoinSuccess.h"
#include "../../common/network/messages/game/outgame/StartGame.h"
#include "../../common/network/messages/enum/CellType.h"
#include "../../common/network/messages/game/ingame/cell/creation/CreateCellRequest.h"
#include "../../common/network/messages/game/ingame/cell/creation/CreateCellFailure.h"
#include "../../common/network/messages/game/ingame/cell/creation/CreateCellSuccess.h"
#include "../../common/network/messages/game/ingame/cell/creation/CellNew.h"

#include "../../common/network/messages/game/ingame/polypeptide/creation/CreatePolypeptideRequest.h"
#include "../../common/network/messages/game/ingame/polypeptide/creation/CreatePolypeptideSuccess.h"
#include "../../common/network/messages/game/ingame/polypeptide/creation/CreatePolypeptideFailure.h"
#include "../../common/network/messages/game/ingame/polypeptide/travel/MovePolypeptideRequest.h"
#include "../../common/network/messages/game/ingame/polypeptide/travel/MovePolypeptideSuccess.h"
#include "../../common/network/messages/game/ingame/polypeptide/travel/MovePolypeptideFailure.h"

#include "../../common/Config.h"
#include "../../common/ConfigurationDataHandler.h"
#include "../../common/GameObjectContainer.h"

#include "../event/EventManager.h"
#include "../event/EventCreator.h"

#include "../environment/BarrierServer.h"
#include "../environment/StaticModificatorServer.h"

#include "CellServer.h"
#include "PolypeptideServer.h"
#include "PlayerServer.h"
#include "GameContext.h"

using namespace std;

Game::Game()
{
	LOG_INFO("Game created");
		
	stringstream message;
	message << "Space for " << CONFIG_INT2("data.players.max", 4) << " players.";
	LOG_INFO(message.str());

	/**
		* Build Modifiers and Barriers
		*/

	vector<float> xPosStatics2 = CONFIG_FLOATS2("data.environment.modifiers.static", "nutrientsoil.position.x");
	vector<float> yPosStatics2 = CONFIG_FLOATS2("data.environment.modifiers.static", "nutrientsoil.position.y");
	vector<float> xRotStatics2 = CONFIG_FLOATS2("data.environment.modifiers.static", "nutrientsoil.rotation.x");
	vector<float> yRotStatics2 = CONFIG_FLOATS2("data.environment.modifiers.static", "nutrientsoil.rotation.y");
	vector<float> radiusStatics2 = CONFIG_FLOATS2("data.environment.modifiers.static", "nutrientsoil.radius");

	if (xPosStatics2.size() != yPosStatics2.size() ||
		xPosStatics2.size() != xRotStatics2.size() ||
		xPosStatics2.size() != yRotStatics2.size() ||
		xPosStatics2.size() != radiusStatics2.size())
	{
		throw string("Xml structure of the nutrientsoil is inconsistent");
	}
	unsigned int nrNutrientsoil = xPosStatics2.size();

	vector<float> xPosStatics3 = CONFIG_FLOATS2("data.environment.modifiers.static", "radioactivity.position.x");
	vector<float> yPosStatics3 = CONFIG_FLOATS2("data.environment.modifiers.static", "radioactivity.position.y");
	vector<float> xRotStatics3 = CONFIG_FLOATS2("data.environment.modifiers.static", "radioactivity.rotation.x");
	vector<float> yRotStatics3 = CONFIG_FLOATS2("data.environment.modifiers.static", "radioactivity.rotation.y");
	vector<float> radiusStatics3 = CONFIG_FLOATS2("data.environment.modifiers.static", "radioactivity.radius");

	if (xPosStatics3.size() != yPosStatics3.size() ||
		xPosStatics3.size() != xRotStatics3.size() ||
		xPosStatics3.size() != yRotStatics3.size() ||
		xPosStatics3.size() != radiusStatics3.size())
	{
		throw string("Xml structure of the radioactivity is inconsistent");
	}
	unsigned int nrRadioactivity = xPosStatics3.size();

	vector<float> xPosStatics;
	xPosStatics.insert(xPosStatics.end(), xPosStatics2.begin(), xPosStatics2.end());
	xPosStatics.insert(xPosStatics.end(), xPosStatics3.begin(), xPosStatics3.end());
	vector<float> yPosStatics;
	yPosStatics.insert(yPosStatics.end(), yPosStatics2.begin(), yPosStatics2.end());
	yPosStatics.insert(yPosStatics.end(), yPosStatics3.begin(), yPosStatics3.end());
	vector<float> xRotStatics;
	xRotStatics.insert(xRotStatics.end(), xRotStatics2.begin(), xRotStatics2.end());
	xRotStatics.insert(xRotStatics.end(), xRotStatics3.begin(), xRotStatics3.end());
	vector<float> yRotStatics;
	yRotStatics.insert(yRotStatics.end(), yRotStatics2.begin(), yRotStatics2.end());
	yRotStatics.insert(yRotStatics.end(), yRotStatics3.begin(), yRotStatics3.end());
	vector<float> radiusStatics;
	radiusStatics.insert(radiusStatics.end(), radiusStatics2.begin(), radiusStatics2.end());
	radiusStatics.insert(radiusStatics.end(), radiusStatics3.begin(), radiusStatics3.end());

	for (unsigned int i = 0; i < xPosStatics.size(); ++i)
	{
		StaticModificator::Type type;
		if (i < nrNutrientsoil)
		{
			type = StaticModificator::NUTRIENTSOIL;
		}
		else
		{
			type = StaticModificator::RADIOACTIVITY;
		}

		StaticModificatorServer * s = new StaticModificatorServer(Vec3f(xPosStatics[i], yPosStatics[i], 0.f), Vec3f(xRotStatics[i], yRotStatics[i], 0.f), Vec3f::one(), radiusStatics[i], type);
		if (!isInRadiusOf<float>(s->getPosition(), s->getRadius(), Vec3f::zero(), CONFIG_FLOAT1("data.world.radius")))
		{
			throw string("Static modifier could not be created because it is not in the game area");
		}
				
		GAMECONTEXT->getEnvironment().createGameObject(s);
	}
	
	vector<float> xPosBarrier = CONFIG_FLOATS2("data.environment.barriers", "barrier.position.x");
	vector<float> yPosBarrier = CONFIG_FLOATS2("data.environment.barriers", "barrier.position.y");
	vector<float> xRotBarrier = CONFIG_FLOATS2("data.environment.barriers", "barrier.rotation.x");
	vector<float> yRotBarrier = CONFIG_FLOATS2("data.environment.barriers", "barrier.rotation.y");
	vector<float> radiusBarrier = CONFIG_FLOATS2("data.environment.barriers", "barrier.radius");

	if (xPosBarrier.size() != yPosBarrier.size() ||
		xPosBarrier.size() != xRotBarrier.size() ||
		xPosBarrier.size() != yRotBarrier.size() ||
		xPosBarrier.size() != radiusBarrier.size())
	{
		throw string("Xml structure of the barriers is inconsistent");
	}

	for (unsigned int i = 0; i < xPosBarrier.size(); ++i)
	{
		BarrierServer * b = new BarrierServer(Vec3f(xPosBarrier[i], yPosBarrier[i], 0.f), Vec3f(xRotBarrier[i], yRotBarrier[i], 0.f), Vec3f::one(), radiusBarrier[i]);
		if (!isInRadiusOf<float>(b->getPosition(), b->getRadius(), Vec3f::zero(), CONFIG_FLOAT1("data.world.radius")))
		{
			throw string("Barrier could not be created because it is not in the game area");
		}
		vector<GameObject *> gameObjects;

		const vector<GameObject *> & activeCells = GAMECONTEXT->getActiveCells().findInRadiusOf(b->getPosition(), b->getRadius());
		const vector<GameObject *> & inactiveCells = GAMECONTEXT->getInactiveCells().findInRadiusOf(b->getPosition(), b->getRadius());
		const vector<GameObject *> & environment = GAMECONTEXT->getEnvironment().findInRadiusOf(b->getPosition(), b->getRadius());
				
		gameObjects.insert(gameObjects.end(), activeCells.begin(), activeCells.end());
		gameObjects.insert(gameObjects.end(), inactiveCells.begin(), inactiveCells.end());

		if (gameObjects.size() > 0)
		{
			throw string("Barrier could not be created because a gameobject is already on this spot");
		}
		GAMECONTEXT->getEnvironment().createGameObject(b);
	}
}

void Game::join(JoinRequest &request)
{
	LOG_INFO("JoinRequest received");
	string playerName = request.name;
		
	if(GAMECONTEXT->getPlayerMap().size() == CONFIG_INT2("data.players.max", 4))
	{
		JoinFailure * failure = new JoinFailure(request);
		failure->errorCode = JoinErrorCode::GameIsFull;
		NETWORKMANAGER->send(failure);
		LOG_INFO("JoinFailure GameIsFull sent");

		LOG_INFO(stringify(ostringstream() << "Player " << playerName << " tried to join the game. But game is full"));

		return;
	}

	for (auto it = GAMECONTEXT->getPlayerMap().begin(); it != GAMECONTEXT->getPlayerMap().end(); ++it)
	{
		if (it->second->getName() == playerName)
		{
			JoinFailure * failure = new JoinFailure(request);
			failure->errorCode = JoinErrorCode::NameAlreadyTaken;
			NETWORKMANAGER->send(failure);
			LOG_INFO("JoinFailure NameAlreadyTaken sent");

			LOG_INFO(stringify(ostringstream() << "Playername " << playerName << " already exists"));

			return;
		}
	}

	/// get the players startposition
	vector<float> xPositions = CONFIG_FLOATS2("data.players", "player.startposition.x");
	vector<float> yPositions = CONFIG_FLOATS2("data.players", "player.startposition.y");
	Vec3f startPosition(xPositions[GAMECONTEXT->getPlayerMap().size()], yPositions[GAMECONTEXT->getPlayerMap().size()], 0.f);

	/// get the stemcell- and world-radius information
	float stemcellRadius = CONFIG_FLOAT1("data.cell.stemcell.radius");
	float worldRadius = CONFIG_FLOAT1("data.world.radius");

	/// test if the stemcell is inside the world
	if (!isInRadiusOf(startPosition, stemcellRadius, Vec3f(0.f, 0.f, 0.f), worldRadius))
	{
		throw string("Startposition of next player does not lie within the world");
	}

	PlayerServer * p = new PlayerServer(playerName, request.endpoint, startPosition);
	LOG_INFO(stringify(ostringstream() << "Player " << GAMECONTEXT->getPlayerMap().size() <<
		" with ID: " << p->getId() << " (" << playerName << 
		") joined the game at the position: (" << startPosition.x << "/" << startPosition.y << ")"));

	GAMECONTEXT->addPlayer(p);

	GAMECONTEXT->getActiveCells().createGameObject(&(p->getStemCell()));

	JoinSuccess *success = new JoinSuccess();
	success->playerId = p->getId();
	success->endpoint = request.endpoint;
	NETWORKMANAGER->send(success);
	LOG_INFO("JoinSuccess sent");

	if (GAMECONTEXT->getPlayerMap().size() == CONFIG_INT2("data.players.max", 4))
	{
		using boost::asio::ip::udp;

		StartGame * startgame = new StartGame();
		startgame->worldRadius = worldRadius;
		for (auto it = GAMECONTEXT->getPlayerMap().begin(); it != GAMECONTEXT->getPlayerMap().end(); ++it)
		{
			NetworkPlayer networkPlayer;
			networkPlayer.playerId = it->second->getId();
			networkPlayer.playerName = it->second->getName();

			networkPlayer.startCellId = it->second->getStemCell().getId();
			networkPlayer.startPosition = it->second->getStemCell().getPosition();

			startgame->players.push_back(networkPlayer);
		}

		/**
		 * Add Modifiers and Barriers to message
		 */

		for (auto it = GAMECONTEXT->getEnvironment().begin(); it != GAMECONTEXT->getEnvironment().end(); ++it)
		{
			BarrierServer * barrier = dynamic_cast<BarrierServer *>(it->second);
			if (barrier)
			{
				NetworkBarrier networkBarrier;
				networkBarrier.modifierId = barrier->getId();
				networkBarrier.position = barrier->getPosition();
				networkBarrier.rotation = barrier->getRotation();
				networkBarrier.scale = barrier->getScale();
				networkBarrier.radius = barrier->getRadius();
				startgame->barriers.push_back(networkBarrier);
			}
			
			StaticModificatorServer * staticModifier = dynamic_cast<StaticModificatorServer *>(it->second);
			if (staticModifier)
			{
				NetworkStaticModifier networkStatic;
				networkStatic.modifierId = staticModifier->getId();
				networkStatic.position = staticModifier->getPosition();
				networkStatic.rotation = staticModifier->getRotation();
				networkStatic.scale = staticModifier->getScale();
				networkStatic.radius = staticModifier->getRadius();

				switch(staticModifier->getType())
				{
				case StaticModificatorServer::RADIOACTIVITY:
					networkStatic.type = StaticModifierType::RadioActivity;
					break;
				case StaticModificatorServer::NUTRIENTSOIL:
					networkStatic.type = StaticModifierType::NutrientSoil;
					break;
				default:
					networkStatic.type = StaticModifierType::Invalid;
					break;
				}

				startgame->staticModifiers.push_back(networkStatic);
			}
		}
			
		NETWORKMANAGER->sendTo<StartGame>(startgame, NETWORKMANAGER->getConnectionEndpoints());
		LOG_INFO("StartGame sent");

		LOG_INFO("Game started");
	}
}

void Game::createCell(CreateCellRequest & request)
{
	LOG_INFO("CreateCellRequest received");
	unsigned int playerId = request.playerId;
	unsigned int cellId = request.cellId;
	float angle = request.angle;
	CellType type = request.type;

	if (GAMECONTEXT->getPlayer(playerId))
	{
		PlayerServer & player = *(GAMECONTEXT->getPlayer(playerId));

		if (player.isDead())
		{
			CreateCellFailure *failure = new CreateCellFailure();
			failure->endpoint = player.getEndpoint();
			failure->requestId = request.requestId;
			failure->errorCode = CreateCellErrorCode::PlayerIsSpectator;
			NETWORKMANAGER->send(failure);
			LOG_INFO("CreateCellFailure PlayerIsSpectator sent");
			return;
		}


		CellServer * parentCell = dynamic_cast<CellServer *>(GAMECONTEXT->getActiveCells().find(cellId));
		if (parentCell == 0)
		{
			LOG_INFO(stringify(ostringstream() << "Cell with the id " << cellId << " does not exist"));
			return;
		}
			
		CellServer * cell = 0;
		Vec3f position;
		switch (type.getType())
		{
		case CellType::StemCell:
			parentCell->getNextCellPositionByAngle(angle, CONFIG_FLOAT1("data.cell.stemcell.radius"), position);
			cell = new CellServer(CellServer::STEMCELL, position, angle, &player);
			break;
		case CellType::StandardCell:
			parentCell->getNextCellPositionByAngle(angle, CONFIG_FLOAT1("data.cell.standardcell.radius"), position);
			cell = new CellServer(CellServer::STANDARDCELL, position, angle, &player);
			break;
		case CellType::BoneCell:
			parentCell->getNextCellPositionByAngle(angle, CONFIG_FLOAT1("data.cell.bonecell.radius"), position);
			cell = new CellServer(CellServer::BONECELL, position, angle, &player);
			break;
		default:
			LOG_INFO(stringify(ostringstream() << "Unknown CellType: " << type.getType()));
			return;
		}

		/// get current time
		double time = EVENT_MGR->getTime();

		if (EVENT_CRTR->createBuildEvent(time, request.requestId, type.getType(), angle, player, *parentCell, *cell))
		{
			string typeName;
			switch(type.getType())
			{
				case CellType::StemCell:
					typeName = "StemCell";
					break;
				case CellType::StandardCell:
					typeName = "StandardCell";
					break;
				case CellType::BoneCell:
					typeName = "BoneCell";
					break;
			}
			LOG_INFO(stringify(ostringstream() << typeName << " of player " << player.getName() << " at position (" << 
				cell->getPosition().x << "/" << cell->getPosition().y << ") will be created"));
		}
		else
		{
			/// creation failed
			if (cell) delete cell;
			return;
		}
	}
	else
	{
		LOG_INFO("Players Id is invalid");
	}
}

void Game::createPolypetide(CreatePolipeptideRequest & request)
{
	LOG_INFO("CreatePolyPeptideRequest received");
	unsigned int playerId = request.playerId;

	PlayerServer * player = testPlayer(playerId);
	if (player == nullptr)
	{
		/// No valid player
		CreatePolypeptideFailure * message = new CreatePolypeptideFailure();
		message->requestId = request.requestId;
		message->errorCode = CreatePolypeptideErrorCode::InvalidPlayer;
		message->endpoint = player->getEndpoint();
		NETWORKMANAGER->send(message);
		
		return;
	}

	CellServer * stemCell = &(player->getStemCell());

	PolypeptideServer * polypetide = new PolypeptideServer(stemCell->getPosition(), stemCell->getAngle(), player);

	if (stemCell->addPolypetide(polypetide))
	{
		/// success
		CreatePolypeptideSuccess * message = new CreatePolypeptideSuccess();
		message->requestId = request.requestId;
		message->endpoint = player->getEndpoint();
		NETWORKMANAGER->send(message);
	}
	else
	{
		/// failure
		CreatePolypeptideFailure * message = new CreatePolypeptideFailure();
		message->requestId = request.requestId;
		message->errorCode = CreatePolypeptideErrorCode::CellFull;
		message->endpoint = player->getEndpoint();
		NETWORKMANAGER->send(message);
	}
}


void Game::movePolypetide(MovePolipeptideRequest & request)
{
	LOG_INFO("MovePolypetideRequest received");
	unsigned int fromCellId = request.fromCellId;
	unsigned int toCellId = request.toCellId;
	unsigned int amount = request.amount;

	CellServer * fromCell = dynamic_cast<CellServer *>(GAMECONTEXT->getActiveCells().find(fromCellId));
	CellServer * toCell = dynamic_cast<CellServer *>(GAMECONTEXT->getActiveCells().find(toCellId));

	if (fromCell == nullptr || toCell == nullptr)
	{
		/// either sending cell or receiving cell are not valid
		MovePolypeptideFailure * message = new MovePolypeptideFailure();
		message->requestId = request.requestId;
		message->errorCode = MovePolypeptideErrorCode::InvalidCells;
		message->endpoint = GAMECONTEXT->getPlayer(fromCell->getOwner()->getId())->getEndpoint();
		NETWORKMANAGER->send(message);
		return;
	}

	if (fromCell->getOwner() != toCell->getOwner())
	{
		/// the cells do not have the same owner
		MovePolypeptideFailure * message = new MovePolypeptideFailure();
		message->requestId = request.requestId;
		message->errorCode = MovePolypeptideErrorCode::DifferentOwner;
		message->endpoint = GAMECONTEXT->getPlayer(fromCell->getOwner()->getId())->getEndpoint();
		NETWORKMANAGER->send(message);
	}

	if (toCell->getPolypeptides().size() + amount >= CONFIG_INT1("data.polypetide.maxPerCell"))
	{
		/// target-cell has not enough space
		MovePolypeptideFailure * message = new MovePolypeptideFailure();
		message->requestId = request.requestId;
		message->errorCode = MovePolypeptideErrorCode::TargetCellFull;
		message->endpoint = GAMECONTEXT->getPlayer(fromCell->getOwner()->getId())->getEndpoint();
		NETWORKMANAGER->send(message);
	}

	vector<unsigned int> polypeptideIds;
	for (auto it = fromCell->getPolypeptides().begin(); it != fromCell->getPolypeptides().end(); ++it)
	{
		polypeptideIds.push_back(it->second->getId());
	}

	for (auto it = polypeptideIds.begin(); it != polypeptideIds.end(); ++it)
	{
		Polypeptide * polypeptide = fromCell->getPolypeptides().find(*it)->second;
		toCell->addPolypetide(polypeptide);
		fromCell->removePolypetide(polypeptide);
	}

	/// send success
	MovePolypeptideSuccess * message = new MovePolypeptideSuccess();
	message->requestId = request.requestId;
	message->polypeptideIds = polypeptideIds;
	NETWORKMANAGER->sendTo<MovePolypeptideSuccess>(message, NETWORKMANAGER->getConnectionEndpoints());
}
	
PlayerServer* Game::testPlayer(unsigned int playerId)
{
	PlayerServer * player = GAMECONTEXT->getPlayer(playerId);
	if (player != nullptr && !player->isDead())
	{
		return player;
	}
	return nullptr;
}

CellServer* Game::testCell(unsigned int cellId)
{
	return dynamic_cast<CellServer *>(GAMECONTEXT->getActiveCells().find(cellId));
}