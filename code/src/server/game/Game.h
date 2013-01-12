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

#include "../event/EventManager.h"
#include "../event/EventCreator.h"

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

		CONFIG_MGR->readFromXML("config.xml");
		LOG_INFO("config.xml loaded");

		players.reserve(CONFIG_INT1("data.players.max"));		
		LOG_INFO(stringify(ostringstream() << "Space for " << players.capacity() << " players reserved"));
	}

	void bind(NetworkManager* networkManager)
	{
		this->networkManager = networkManager;
		LOG_INFO("NetworkManager bound to game");
	}

	void join(JoinRequest request)
	{
		string playerName = request.name;
		
		if(players.size() == players.capacity())
		{
			JoinFailure * failure = new JoinFailure();
			failure->errorCode = JoinErrorCode::GameIsFull;
			networkManager->send(failure);

			LOG_INFO(stringify(ostringstream() << "Player " << playerName << " tried to join the game. But game is full"));

			return;
		}

		for (auto it = players.begin(); it != players.end(); ++it)
		{
			if ((*it)->getName() == playerName)
			{
				JoinFailure * failure = new JoinFailure();
				failure->errorCode = JoinErrorCode::NameAlreadyTaken;
				networkManager->send(failure);

				LOG_INFO(stringify(ostringstream() << "Playername " << playerName << " already exists"));

				return;
			}
		}

		/// get the players startposition
		vector<float> xPositions = CONFIG_FLOATS2("data.players", "player.startposition.x");
		vector<float> yPositions = CONFIG_FLOATS2("data.players", "player.startposition.y");
		Vec3f startPosition(xPositions[players.size()], yPositions[players.size()], 0.f);

		/// get the stemcell- and world-radius information
		float stemcellRadius = CONFIG_FLOAT1("data.cell.stemcell.radius");
		float worldRadius = CONFIG_FLOAT1("data.world.radius");

		/// test if the stemcell is inside the world
		if (!isInRadiusOf(startPosition, stemcellRadius, Vec3f(0.f, 0.f, 0.f), worldRadius))
		{
			throw string("Startposition of next player does not lie within the world");
		}

		PlayerServer * p = new PlayerServer(playerName, request.endpoint, startPosition);
		LOG_INFO(stringify(ostringstream() << "Player " << players.size() << "(" << playerName << 
			") joined the game at the position: (" << startPosition.x << "/" << startPosition.y << ")"));

		players.push_back(p);

		JoinSuccess * success = new JoinSuccess();
		success->playerId = p->getId();
		success->endpoint = request.endpoint;
		networkManager->send(success);

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
				LOG_INFO(stringify(ostringstream() << "Cell with the id " << cellId << " does not exist"));
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
			double time = EVENT_MGR->getTime();

			if (EVENT_CRTR->createBuildEvent(time, request.requestId, type.getType(), angle, player, *cell))
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
				}
				LOG_INFO(stringify(ostringstream() << typeName << " of player " << player.getName() << " at position (" << 
					cell->getPosition().x << "/" << cell->getPosition().y << ") will be created"));
			}
			else
			{
				/// creation failed
				return;
			}

			if (!EVENT_CRTR->createAttackEvent(time, false, player, *cell))
			{
				/// no attacks are performed
				return;
			}
		}
		LOG_INFO("Players Id is invalid");
	}

	
private:
	NetworkManager* networkManager;
	vector<PlayerServer*> players;
	GameObjectContainer gameObjectContainer;
};