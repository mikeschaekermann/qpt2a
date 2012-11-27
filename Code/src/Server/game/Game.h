#pragma once

#include <string>
#include <vector>
#include <cmath>

#include "../network/NetworkManager.h"
#include "../network/JoinFailure.h"
#include "../network/JoinSuccess.h"
#include "../network/StartGame.h"
#include "../network/CellType.h"
#include "../network/CreateCellFailure.h"

#include "../event/BuildingEvent.h"
#include "../event/EventQueue.h"

#include "Player.h"

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

		m_pafStartPositions = new float[MAX_PLAYER_SIZE][2];
		for (unsigned int i = 0; i < MAX_PLAYER_SIZE; ++i)
		{
			m_pafStartPositions[i][0] = cosf(i * (360.f / MAX_PLAYER_SIZE) * (float)M_PI / 180.f) * PLAYER_START_DISTANCE;
			m_pafStartPositions[i][1] = sinf(i * (360.f / MAX_PLAYER_SIZE) * (float)M_PI / 180.f) * PLAYER_START_DISTANCE;
		}


		cout << "Game created" << endl;
	}

	void bindNetworkManager(NetworkManager* networkManager)
	{
		m_pNetworkManager = networkManager;
		cout << "NetworkManager bound to Game" << endl;
	}

	void join(string playerName, string ipAddress, unsigned short port)
	{
		if(m_players.size() >= MAX_PLAYER_SIZE)
		{
			JoinFailure failure;
			failure.errorCode = JoinErrorCode(JoinErrorCode::Type::GameIsFull);
			m_pNetworkManager->send(failure);
			return;
		}

		vector<Player>::iterator it = m_players.begin();
		for (; it != m_players.end(); ++it)
		{
			if (it->getName() == playerName)
			{
				JoinFailure failure;
				failure.errorCode = JoinErrorCode(JoinErrorCode::Type::NameAlreadyTaken);
				m_pNetworkManager->send(failure);
				return;
			}
		}

		Player p(createPlayerId(), playerName, ipAddress, port, m_pafStartPositions[m_players.size()]);

		m_players.push_back(p);

		JoinSuccess success;
		success.playerId = p.getId();
		m_pNetworkManager->send(success);

		if (m_players.size() == MAX_PLAYER_SIZE)
		{
			StartGame startgame;
			startgame.worldRadius = m_fWorldRadius;
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

				startgame.startPositions[i] = new float[2];
				startgame.startPositions[i][0] = m_players[i].getPopulation().find((unsigned int)0)->getPosition()[0];
				startgame.startPositions[i][1] = m_players[i].getPopulation().find((unsigned int)0)->getPosition()[1];
			}

			m_pNetworkManager->send(startgame);
		}
	}

	void createCell(unsigned int playerId, unsigned int cellId, float angle, CellType type)
	{
		if (playerId < m_players.size())
		{
			Player& player = m_players[playerId];
			Cell* parentCell = player.getPopulation().find(cellId);
			if (parentCell == 0)
			{
				/// cell does not exist
				return;
			}
			
			const float* const position = player.getPopulation().getRelativePosition(cellId, angle);
			Cell* cell = 0;
			switch (type.getType())
			{
			case CellType::StemCell:
				cell = new Cell();
			case CellType::StandardCell:
				cell = new Cell();
			default:
				cell = 0;
			}

			for (unsigned int i = 0; i < m_players.size(); ++i)
			{
				const vector<Cell*>& cells = 
					m_players[i].getPopulation().findInRadiusOf(position, cell->getRadius());
				if (cells.size() > 0)
				{
					/// collision detected
					return;
				}
			}
			player.getPopulation().createCell(cell, position);
			/// add building event to event queue
			BuildingEvent* e = new BuildingEvent();
			m_pEventQueue->addEvent(e);

			delete position;
		}
	}
private:
	NetworkManager* m_pNetworkManager;
	EventQueue* m_pEventQueue;
	vector<Player> m_players;
	float (*m_pafStartPositions)[2]; ///< pointer of arrays
	float m_fWorldRadius;

	unsigned int createPlayerId()
	{
		return m_players.size();
	}
};