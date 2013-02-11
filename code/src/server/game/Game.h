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

#include "../environment/BarrierServer.h"
#include "../environment/StaticModificatorServer.h"
#include "../environment/DynamicModificatorServer.h"

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
		players.reserve(CONFIG_INT2("data.players.max", 4));
		
		stringstream message;
		message << "Space for " << players.capacity() << " players reserved";
		LOG_INFO(message.str());
	}

	void bind(NetworkManager* networkManager)
	{
		this->networkManager = networkManager;
		LOG_INFO("NetworkManager bound to game");

		EventCreator::getInstance()->bind(networkManager, &gameObjectContainer, &(this->players));
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

		gameObjectContainer.createGameObject(&(p->getStemCell()));

		JoinSuccess *success = new JoinSuccess();
		success->playerId = p->getId();
		success->endpoint = request.endpoint;
		networkManager->send(success);

		if (players.size() == players.capacity())
		{
			using boost::asio::ip::udp;

			/**
			 * Build Modifiers and Barriers
			 */

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
				const vector<GameObject *> & gameObjects = gameObjectContainer.findInRadiusOf(b->getPosition(), b->getRadius());
				if (gameObjects.size() > 0)
				{
					throw string("Barrier could not be created because a gameobject is already on this spot");
				}
				gameObjectContainer.createGameObject(b);
			}

			vector<float> xPosStatics1 = CONFIG_FLOATS2("data.environment.modifiers.static", "antibiotic.position.x");
			vector<float> yPosStatics1 = CONFIG_FLOATS2("data.environment.modifiers.static", "antibiotic.position.y");
			vector<float> xRotStatics1 = CONFIG_FLOATS2("data.environment.modifiers.static", "antibiotic.rotation.x");
			vector<float> yRotStatics1 = CONFIG_FLOATS2("data.environment.modifiers.static", "antibiotic.rotation.y");
			vector<float> radiusStatics1 = CONFIG_FLOATS2("data.environment.modifiers.static", "antibiotic.radius");

			if (xPosStatics1.size() != yPosStatics1.size() ||
				xPosStatics1.size() != xRotStatics1.size() ||
				xPosStatics1.size() != yRotStatics1.size() ||
				xPosStatics1.size() != radiusStatics1.size())
			{
				throw string("Xml structure of the antibiotics is inconsistent");
			}
			unsigned int nrAntibiotics = xPosStatics1.size();

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
			xPosStatics.insert(xPosStatics.begin(), xPosStatics1.begin(), xPosStatics1.end());
			xPosStatics.insert(xPosStatics.end(), xPosStatics2.begin(), xPosStatics2.end());
			xPosStatics.insert(xPosStatics.end(), xPosStatics3.begin(), xPosStatics3.end());
			vector<float> yPosStatics;
			yPosStatics.insert(yPosStatics.begin(), yPosStatics1.begin(), yPosStatics1.end());
			yPosStatics.insert(yPosStatics.end(), yPosStatics2.begin(), yPosStatics2.end());
			yPosStatics.insert(yPosStatics.end(), yPosStatics3.begin(), yPosStatics3.end());
			vector<float> xRotStatics;
			xRotStatics.insert(xRotStatics.begin(), xRotStatics1.begin(), xRotStatics1.end());
			xRotStatics.insert(xRotStatics.end(), xRotStatics2.begin(), xRotStatics2.end());
			xRotStatics.insert(xRotStatics.end(), xRotStatics3.begin(), xRotStatics3.end());
			vector<float> yRotStatics;
			yRotStatics.insert(yRotStatics.begin(), yRotStatics1.begin(), yRotStatics1.end());
			yRotStatics.insert(yRotStatics.end(), yRotStatics2.begin(), yRotStatics2.end());
			yRotStatics.insert(yRotStatics.end(), yRotStatics3.begin(), yRotStatics3.end());
			vector<float> radiusStatics;
			radiusStatics.insert(radiusStatics.begin(), radiusStatics1.begin(), radiusStatics1.end());
			radiusStatics.insert(radiusStatics.end(), radiusStatics2.begin(), radiusStatics2.end());
			radiusStatics.insert(radiusStatics.end(), radiusStatics3.begin(), radiusStatics3.end());

			for (unsigned int i = 0; i < xPosStatics.size(); ++i)
			{
				StaticModificator::Type type;
				if (i < nrAntibiotics)
				{
					type = StaticModificator::ANTIBIOTIC;
				}
				else if (i < nrAntibiotics + nrNutrientsoil)
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
				const vector<GameObject *> & gameObjects = gameObjectContainer.findInRadiusOf(s->getPosition(), s->getRadius());
				if (gameObjects.size() > 0)
				{
					throw string("Static modifier could not be created because a gameobject is already on this spot");
				}
				gameObjectContainer.createGameObject(s);
			}

			vector<float> xPosDynamics1 = CONFIG_FLOATS2("data.environment.modifiers.static", "viriswarm.position.x");
			vector<float> yPosDynamics1 = CONFIG_FLOATS2("data.environment.modifiers.static", "viriswarm.position.y");
			vector<float> xRotDynamics1 = CONFIG_FLOATS2("data.environment.modifiers.static", "viriswarm.rotation.x");
			vector<float> yRotDynamics1 = CONFIG_FLOATS2("data.environment.modifiers.static", "viriswarm.rotation.y");
			vector<float> radiusDynamics1 = CONFIG_FLOATS2("data.environment.modifiers.static", "viriswarm.radius");

			if (xPosDynamics1.size() != yPosDynamics1.size() ||
				xPosDynamics1.size() != xRotDynamics1.size() ||
				xPosDynamics1.size() != yRotDynamics1.size() ||
				xPosDynamics1.size() != radiusDynamics1.size())
			{
				throw string("Xml structure of the viriswarm is inconsistent");
			}
			unsigned int nrViriswarm = xPosDynamics1.size();

			vector<float> xPosDynamics2 = CONFIG_FLOATS2("data.environment.modifiers.static", "vitamingeyser.position.x");
			vector<float> yPosDynamics2 = CONFIG_FLOATS2("data.environment.modifiers.static", "vitamingeyser.position.y");
			vector<float> xRotDynamics2 = CONFIG_FLOATS2("data.environment.modifiers.static", "vitamingeyser.rotation.x");
			vector<float> yRotDynamics2 = CONFIG_FLOATS2("data.environment.modifiers.static", "vitamingeyser.rotation.y");
			vector<float> radiusDynamics2 = CONFIG_FLOATS2("data.environment.modifiers.static", "vitamingeyser.radius");

			if (xPosDynamics2.size() != yPosDynamics2.size() ||
				xPosDynamics2.size() != xRotDynamics2.size() ||
				xPosDynamics2.size() != yRotDynamics2.size() ||
				xPosDynamics2.size() != radiusDynamics2.size())
			{
				throw string("Xml structure of the vitamingeyser is inconsistent");
			}
			unsigned int nrVitamingeyser = xPosDynamics2.size();

			vector<float> xPosDynamics3 = CONFIG_FLOATS2("data.environment.modifiers.static", "loosefatcell.position.x");
			vector<float> yPosDynamics3 = CONFIG_FLOATS2("data.environment.modifiers.static", "loosefatcell.position.y");
			vector<float> xRotDynamics3 = CONFIG_FLOATS2("data.environment.modifiers.static", "loosefatcell.rotation.x");
			vector<float> yRotDynamics3 = CONFIG_FLOATS2("data.environment.modifiers.static", "loosefatcell.rotation.y");
			vector<float> radiusDynamics3 = CONFIG_FLOATS2("data.environment.modifiers.static", "loosefatcell.radius");

			if (xPosDynamics3.size() != yPosDynamics3.size() ||
				xPosDynamics3.size() != xRotDynamics3.size() ||
				xPosDynamics3.size() != yRotDynamics3.size() ||
				xPosDynamics3.size() != radiusDynamics3.size())
			{
				throw string("Xml structure of the loosefatcell is inconsistent");
			}
			unsigned int nrLoosefatcell = xPosDynamics3.size();

			vector<float> xPosDynamics;
			xPosDynamics.insert(xPosDynamics.begin(), xPosDynamics1.begin(), xPosDynamics1.end());
			xPosDynamics.insert(xPosDynamics.end(), xPosDynamics2.begin(), xPosDynamics2.end());
			xPosDynamics.insert(xPosDynamics.end(), xPosDynamics3.begin(), xPosDynamics3.end());
			vector<float> yPosDynamics;
			yPosDynamics.insert(yPosDynamics.begin(), yPosDynamics1.begin(), yPosDynamics1.end());
			yPosDynamics.insert(yPosDynamics.end(), yPosDynamics2.begin(), yPosDynamics2.end());
			yPosDynamics.insert(yPosDynamics.end(), yPosDynamics3.begin(), yPosDynamics3.end());
			vector<float> xRotDynamics;
			xRotDynamics.insert(xRotDynamics.begin(), xRotDynamics1.begin(), xRotDynamics1.end());
			xRotDynamics.insert(xRotDynamics.end(), xRotDynamics2.begin(), xRotDynamics2.end());
			xRotDynamics.insert(xRotDynamics.end(), xRotDynamics3.begin(), xRotDynamics3.end());
			vector<float> yRotDynamics;
			yRotDynamics.insert(yRotDynamics.begin(), yRotDynamics1.begin(), yRotDynamics1.end());
			yRotDynamics.insert(yRotDynamics.end(), yRotDynamics2.begin(), yRotDynamics2.end());
			yRotDynamics.insert(yRotDynamics.end(), yRotDynamics3.begin(), yRotDynamics3.end());
			vector<float> radiusDynamics;
			radiusDynamics.insert(radiusDynamics.begin(), radiusDynamics1.begin(), radiusDynamics1.end());
			radiusDynamics.insert(radiusDynamics.end(), radiusDynamics2.begin(), radiusDynamics2.end());
			radiusDynamics.insert(radiusDynamics.end(), radiusDynamics3.begin(), radiusDynamics3.end());

			for (unsigned int i = 0; i < xPosDynamics.size(); ++i)
			{
				DynamicModificator::Type type;
				if (i < nrViriswarm)
				{
					type = DynamicModificator::VIRISWARM;
				}
				else if (i < nrViriswarm + nrVitamingeyser)
				{
					type = DynamicModificator::VITAMINGEYSER;
				}
				else
				{
					type = DynamicModificator::LOOSEFATCELL;
				}

				DynamicModificatorServer * d = new DynamicModificatorServer(Vec3f(xPosDynamics[i], yPosDynamics[i], 0.f), Vec3f(xRotDynamics[i], yRotDynamics[i], 0.f), Vec3f::one(), radiusDynamics[i], type);
				if (!isInRadiusOf<float>(d->getPosition(), d->getRadius(), Vec3f::zero(), CONFIG_FLOAT1("data.world.radius")))
				{
					throw string("Dynamic modifier could not be created because it is not in the game area");
				}
				const vector<GameObject *> & gameObjects = gameObjectContainer.findInRadiusOf(d->getPosition(), d->getRadius());
				if (gameObjects.size() > 0)
				{
					throw string("Dynamic modifier could not be created because a gameobject is already on this spot");
				}
				gameObjectContainer.createGameObject(d);
			}

			vector<udp::endpoint> endpointArr;
			StartGame * startgame = new StartGame();
			startgame->worldRadius = worldRadius;
			for (unsigned int i = 0; i < players.size(); ++i)
			{
				NetworkPlayer networkPlayer;
				networkPlayer.playerId = players[i]->getId();
				networkPlayer.playerName = players[i]->getName();

				networkPlayer.startCellId = players[i]->getStemCell().getId();
				networkPlayer.startPosition = players[i]->getStemCell().getPosition();

				startgame->players.push_back(networkPlayer);

				endpointArr.push_back(players[i]->getEndpoint());
			}

			/**
			 * Add Modifiers and Barriers to message
			 */
			
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
				cell = new CellServer(CellServer::STEMCELL, position, angle, &player);
				break;
			case CellType::StandardCell:
				parentCell->getNextCellPositionByAngle(angle, CONFIG_FLOAT1("data.cell.standardcell.radius"), position);
				cell = new CellServer(CellServer::STANDARDCELL, position, angle, &player);
				break;
			default:
				stringstream message;
				message << "Unknown CellType: " << type.getType();
				LOG_INFO(message.str());
				return;
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
	GameObjectContainer<GameObject> gameObjectContainer;
};