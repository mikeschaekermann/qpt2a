#pragma once
#include <string>
#include <sstream>
#include <vector>
#include <cmath>

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

#include "../../common/Config.h"
#include "../../common/Helper.h"
#include "../../common/ConfigurationDataHandler.h"
#include "../../common/GameObjectContainer.h"

#include "../event/EventQueue.h"

#include "EventCreator.h"
#include "CellServer.h"
#include "PlayerServer.h"

using namespace std;

class Game
{
public:
	friend class ServerNetworkManager;
	Game()
	{
		LOG_INFO("Game created");

		ConfigurationDataHandler::getInstance()->readFromXML("..\\..\\config.xml");
		players.reserve(CONFIG_INT2("data.players.max", 4));
		
		stringstream message;
		message << "Space for " << players.capacity() << " players reserved";
		LOG_INFO(message.str());
	}

	void bind(NetworkManager* networkManager, EventQueue* eventQueue)
	{
		this->networkManager = networkManager;
		LOG_INFO("NetworkManager bound to game");

		this->eventQueue = eventQueue;
		LOG_INFO("EventQueue bound to Game");

		//EventCreator::getInstance().bind(m_pNetworkManager, m_pEventQueue, &m_players);
	}

	void join(JoinRequest request)
	{
		stringstream message;
		string playerName = request.name;
		
		if(players.size() == players.capacity())
		{
			JoinFailure *failure = new JoinFailure();
			failure->errorCode = JoinErrorCode::GameIsFull;
			networkManager->send(failure);

			message.clear();
			message << "Player " << playerName << " tried to join the game. But game is full";
			LOG_INFO(message.str());

			return;
		}

		for (auto it = players.begin(); it != players.end(); ++it)
		{
			if ((*it)->getName() == playerName)
			{
				JoinFailure *failure = new JoinFailure();
				failure->errorCode = JoinErrorCode::NameAlreadyTaken;
				networkManager->send(failure);

				message.clear();
				message << "Playername " << playerName << " already exists";
				LOG_INFO(message.str());

				return;
			}
		}

		vector<float> xPositions = ConfigurationDataHandler::getInstance()->getChildrenDataVector<float>("data.players", "player.startposition.x");
		vector<float> yPositions = ConfigurationDataHandler::getInstance()->getChildrenDataVector<float>("data.players", "player.startposition.y");
		Vec3f startPosition(xPositions[players.size()], yPositions[players.size()], 0.f);
		float stemcellRadius = CONFIG_FLOAT1("data.cell.stemcell.radius");
		float worldRadius = CONFIG_FLOAT1("data.world.radius");

		if (!isInRadiusOf(startPosition, stemcellRadius, Vec3f(0.f, 0.f, 0.f), worldRadius))
		{
			LOG_ERROR("Startposition of next player does not lie within the world");
			return;
		}
		PlayerServer *p = new PlayerServer(playerName, request.endpoint, startPosition);

		players.push_back(p);

		JoinSuccess *success = new JoinSuccess();
		success->playerId = p->getId();
		success->endpoint = request.endpoint;
		networkManager->send(success);

		message.clear();
		message << "Player " << playerName << " joined the game";
		LOG_INFO(message.str());

		if (players.size() == players.capacity())
		{
			using boost::asio::ip::udp;

			vector<udp::endpoint> endpointArr;
			StartGame * startgame = new StartGame();
			startgame->worldRadius = worldRadius;
			for (unsigned int i = 0; i < players.size(); ++i)
			{
				NetworkPlayer networkPlayer;
				networkPlayer.playerId = players[i]->getId();
				networkPlayer.playerName = players[i]->getName();

				networkPlayer.startCellId = 0;
				networkPlayer.startPosition = players[i]->getStemCell().getPosition();

				startgame->players.push_back(networkPlayer);

				endpointArr.push_back(players[i]->getEndpoint());
			}
			
			networkManager->sendTo<StartGame>(startgame, endpointArr);

			LOG_INFO("Game started");
		}
	}

	void createCell(CreateCellRequest request)
	{
		stringstream message;
		unsigned int playerId = request.playerId;
		unsigned int cellId = request.cellId;
		float angle = request.angle;
		CellType type = request.type;

		if (playerId < players.size())
		{
			PlayerServer & player = *(players[playerId]);
			CellServer * parentCell = dynamic_cast<CellServer *>(gameObjectContainer.find(cellId));
			if (parentCell == 0)
			{
				message.clear();
				message << "Cell with the id " << cellId << " does not exist";
				LOG_INFO(message.str());
				return;
			}
			
			CellServer * cell = 0;
			Vec3f position;
			switch (type.getType())
			{
			case CellType::StemCell:
				parentCell->getNextCellPositionByAngle(angle, CONFIG_FLOAT1("data.cell.stemcell.radius"), position);
				cell = new CellServer(CellServer::STEMCELL, position, angle);
			case CellType::StandardCell:
				parentCell->getNextCellPositionByAngle(angle, CONFIG_FLOAT1("data.cell.standardcell.radius"), position);
				cell = new CellServer(CellServer::STANDARDCELL, position, angle);
			default:
				cell = 0;
			}

			/// get current time
			double time = eventQueue->getTime();

			if (!EventCreator::getInstance()->createBuildEvent(time, request.requestId, type.getType(), angle, player, *cell))
			{
				/// creation failed
				return;
			}

			if (EventCreator::getInstance()->createAttackEvent(time, false, player, *cell))
			{
				/// no attacks are performed
				return;
			}
		}
		LOG_INFO("Players Id is invalid");
	}

	
private:
	NetworkManager* networkManager;
	EventQueue* eventQueue;
	vector<PlayerServer*> players;
	GameObjectContainer gameObjectContainer;
};