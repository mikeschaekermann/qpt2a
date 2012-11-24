#pragma once

#include <string>
#include <vector>
#include <cmath>

#include "../network/NetworkManager.h"
#include "../network/JoinFailure.h"
#include "../network/JoinSuccess.h"
#include "../network/StartGame.h"
#include "Player.h"

using namespace std;

#define MAX_PLAYER_SIZE 4
#define WORLD_RADIUS 100.0
#define PLAYER_START_DISTANCE 70.0

class Game
{
private:
	NetworkManager* m_networkManager;
	vector<Player> m_players;
	float (*m_startPositions)[3]; ///< pointer of arrays
	float m_worldRadius;

	unsigned int createPlayerId()
	{
		return m_players.size();
	}
public:
	Game() :
		m_networkManager(0),
		m_worldRadius(WORLD_RADIUS)
	{
		m_worldRadius = WORLD_RADIUS;

		m_startPositions = new float[MAX_PLAYER_SIZE][3];
		for (unsigned int i = 0; i < MAX_PLAYER_SIZE; ++i)
		{
			m_startPositions[i][0] = cos(i * (360.0 / MAX_PLAYER_SIZE) * M_PI / 180.0) * PLAYER_START_DISTANCE;
			m_startPositions[i][1] = sin(i * (360.0 / MAX_PLAYER_SIZE) * M_PI / 180.0) * PLAYER_START_DISTANCE;
			m_startPositions[i][2] = 0;
		}


		cout << "Game created" << endl;
	}

	void bindNetworkManager(NetworkManager* networkManager)
	{
		m_networkManager = networkManager;
		cout << "NetworkManager bound to Game" << endl;
	}

	void join(string playerName, string ipAddress, unsigned short port)
	{
		if(m_players.size() >= MAX_PLAYER_SIZE)
		{
			JoinFailure failure;
			failure.errorCode = JoinErrorCode(JoinErrorCode::Type::GameIsFull);
			m_networkManager->send(failure);
			return;
		}

		vector<Player>::iterator it = m_players.begin();
		for (; it != m_players.end(); ++it)
		{
			if (it->getName() == playerName)
			{
				JoinFailure failure;
				failure.errorCode = JoinErrorCode(JoinErrorCode::Type::NameAlreadyTaken);
				m_networkManager->send(failure);
				return;
			}
		}

		Player p(createPlayerId(), playerName, ipAddress, port, m_startPositions[m_players.size()]);

		m_players.push_back(p);

		JoinSuccess success;
		success.playerId = p.getId();
		m_networkManager->send(success);

		if (m_players.size() == MAX_PLAYER_SIZE)
		{
			StartGame startgame;
			startgame.worldRadius = m_worldRadius;
			startgame.playerCount = m_players.size();
			startgame.playerIds = new unsigned int[m_players.size()];
			startgame.playerNames = new char*[m_players.size()];
			startgame.playerNameSizes = new unsigned int[m_players.size()];
			startgame.startCellIds = new unsigned int[m_players.size()];
			startgame.startPositions = new float*[m_players.size()];
			for (unsigned int i = 0; i < m_players.size(); ++i)
			{
				startgame.playerIds[i] = m_players[i].getId();
				startgame.playerNameSizes[i] = m_players[i].getName().length();
				for (unsigned int j = 0; j < startgame.playerNameSizes[i]; ++j)
				{
					startgame.playerNames[i][j] = m_players[i].getName()[j];
				}

				startgame.startCellIds[i] = 0;

				startgame.startPositions[i] = new float[3];
				startgame.startPositions[i][0] = m_players[i].getPopulation().getCellById().getPosition()[0];
				startgame.startPositions[i][1] = m_players[i].getPopulation().getCellById().getPosition()[1];
				startgame.startPositions[i][2] = m_players[i].getPopulation().getCellById().getPosition()[2];
			}
		}
	}
};