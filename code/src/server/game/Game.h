#pragma once
#include <string>
#include <vector>
#include <cmath>

#include "cinder/Vector.h"

#include "../../common/network/NetworkManager.h"
#include "../../common/network/messages/game/outgame/JoinRequest.h"
#include "../../common/network/messages/game/outgame/JoinFailure.h"
#include "../../common/network/messages/game/outgame/JoinSuccess.h"
#include "../../common/network/messages/game/outgame/StartGame.h"
#include "../../common/network/messages/enum/CellType.h"
#include "../../common/network/messages/game/ingame/cell/creation/CreateCellRequest.h"
#include "../../common/network/messages/game/ingame/cell/creation/CreateCellFailure.h"
#include "../../common/network/messages/game/ingame/cell/creation/CreateCellSuccess.h"
#include "../../common/network/messages/game/ingame/cell/creation/CellNew.h"

#include "../event/EventQueue.h"

#include "EventCreator.h"
#include "../../common/Cell.h"
#include "../../common/StemCell.h"
#include "../../common/StandardCell.h"
#include "PlayerServer.h"

using namespace std;

#define MAX_PLAYER_SIZE 4
#define WORLD_RADIUS 100.f
#define PLAYER_START_DISTANCE 70.f

class Game
{
public:
	Game() :
		m_pNetworkManager(0),
		m_fWorldRadius(WORLD_RADIUS)
	{
		m_fWorldRadius = WORLD_RADIUS;

		startPositions.reserve(MAX_PLAYER_SIZE);
		for (unsigned int i = 0; i < MAX_PLAYER_SIZE; ++i)
		{
			startPositions[i] = Vec3f(
				cosf(i * (360.f / MAX_PLAYER_SIZE) * (float)M_PI / 180.f) * PLAYER_START_DISTANCE,
				sinf(i * (360.f / MAX_PLAYER_SIZE) * (float)M_PI / 180.f) * PLAYER_START_DISTANCE,
				0
			);
		}

		cout << "Game created" << endl;
	}

	void bind(NetworkManager* networkManager, EventQueue* eventQueue)
	{
		m_pNetworkManager = networkManager;
		cout << "NetworkManager bound to Game" << endl;

		m_pEventQueue = eventQueue;
		cout << "EventQueue bound to Game" << endl;

		EventCreator::getInstance().bind(m_pNetworkManager, m_pEventQueue, &m_players);
	}

	void join(JoinRequest request)
	{
		string playerName = request.name;
		
		if(m_players.size() >= MAX_PLAYER_SIZE)
		{
			JoinFailure *failure = new JoinFailure();
			failure->errorCode = JoinErrorCode::GameIsFull;
			m_pNetworkManager->send(failure);
			return;
		}

		auto it = m_players.begin();
		for (; it != m_players.end(); ++it)
		{
			if ((*it)->getName() == playerName)
			{
				JoinFailure *failure = new JoinFailure();
				failure->errorCode = JoinErrorCode::NameAlreadyTaken;
				m_pNetworkManager->send(failure);
				return;
			}
		}

		PlayerServer *p = new PlayerServer(PlayerServer::getNewId(), playerName, request.endpoint, startPositions[m_players.size()]);

		m_players.push_back(p);

		JoinSuccess *success = new JoinSuccess();
		success->playerId = p->getId();
		m_pNetworkManager->send(success);

		if (m_players.size() == MAX_PLAYER_SIZE)
		{
			StartGame *startgame = new StartGame();
			startgame->worldRadius = m_fWorldRadius;
			startgame->playerCount = m_players.size();
			startgame->playerIds = new unsigned int[m_players.size()];
			startgame->playerNames = new char*[m_players.size()];
			startgame->playerNameSizes = new unsigned int[m_players.size()];
			startgame->startCellIds = new unsigned int[m_players.size()];
			startgame->startPositions = new float*[m_players.size()];
			for (unsigned int i = 0; i < m_players.size(); ++i)
			{
				startgame->playerIds[i] = m_players[i]->getId();
				startgame->playerNameSizes[i] = m_players[i]->getName().length();
				for (unsigned int j = 0; j < startgame->playerNameSizes[i]; ++j)
				{
					startgame->playerNames[i][j] = m_players[i]->getName()[j];
				}

				startgame->startCellIds[i] = 0;

				startgame->startPositions[i] = new float[2];
				startgame->startPositions[i][0] = m_players[i]->getStemCell.getPoition().x;
				startgame->startPositions[i][1] = m_players[i]->getStemCell.getPoition().y;
			}

			m_pNetworkManager->send(startgame);
		}
	}

	void createCell(CreateCellRequest request)
	{
		unsigned int playerId = request.playerId;
		unsigned int cellId = request.cellId;
		float angle = request.angle;
		CellType type = request.type;

		if (playerId < m_players.size())
		{
			Player& player = *(m_players[playerId]);
			Cell* parentCell = player.getPopulation().find(cellId);
			if (parentCell == 0)
			{
				/// cell does not exist
				return;
			}
			
			Cell* cell = 0;
			switch (type.getType())
			{
			case CellType::StemCell:
				cell = new StemCell();
			case CellType::StandardCell:
				cell = new StandardCell();
			default:
				cell = 0;
			}

			/// get current time
			double time = m_pEventQueue->getTime();

			if (!EventCreator::getInstance().createBuildEvent(time, request.requestId, type.getType(), angle, player, *cell))
			{
				/// creation failed
				return;
			}

			if (EventCreator::getInstance().createAttackEvent(time, false, player, *cell))
			{
				/// no attacks are performed
				return;
			}
		}
	}

	vector<PlayerServer*> m_players;
private:
	NetworkManager* m_pNetworkManager;
	EventQueue* m_pEventQueue;

	vector<Vec3f> startPositions;
	float m_fWorldRadius;
};