#include "EventCreator.h"

#include "../../common/Config.h"

#include "../../common/network/NetworkManager.h"
#include "../../common/network/messages/game/ingame/cell/creation/CreateCellFailure.h"
#include "../../common/network/messages/game/ingame/cell/creation/CellNew.h"
#include "../../common/network/messages/game/ingame/cell/creation/CreateCellSuccess.h"

#include "../../common/GameObjectContainer.h"

#include "../game/PlayerServer.h"
#include "../game/CellServer.h"
#include "../game/GameContext.h"

#include "../environment/StaticModificatorServer.h"

#include "cinder/Rand.h"

#include "GameEvent.h"
#include "EventManager.h"
#include "AttackEvent.h"
#include "BuildingEvent.h"

using namespace std;

EventCreator * EventCreator::instance = 0;

EventCreator * EventCreator::getInstance()
{
	if (instance == nullptr)
	{
		instance = new EventCreator();
	}
	return instance;
}

bool EventCreator::createBuildEvent(const double time, const unsigned int requestId, const int type, const float angle, PlayerServer & currentPlayer, CellServer & cell)
{
	if (!isInRadiusOf<float>(cell.getPosition(), cell.getRadius(), Vec3f::zero(), CONFIG_FLOAT1("data.world.radius")))
	{
		CreateCellFailure *failure = new CreateCellFailure();
		failure->endpoint = currentPlayer.getEndpoint();
		failure->requestId = requestId;
		failure->errorCode = CreateCellErrorCode::OutsideGameArea;
		NETWORKMANAGER->send(failure);
		LOG_INFO("CreateCellFailure OutsideGameArea sent");

		LOG_ERROR("Cell could not be created because it is not in the game area");
		return false;
	}

	auto it = GAMECONTEXT->getPlayerMap().begin();
	for (; it != GAMECONTEXT->getPlayerMap().end(); ++it)
	{
		const vector<GameObject *> & activeCells = GAMECONTEXT->getActiveCells().findInRadiusOf(cell.getPosition(), cell.getRadius());
		const vector<GameObject *> & inactivecells =  GAMECONTEXT->getInactiveCells().findInRadiusOf(cell.getPosition(), cell.getRadius());

		vector<GameObject *> gameObjects;
		gameObjects.insert(gameObjects.end(), activeCells.begin(), activeCells.end());
		gameObjects.insert(gameObjects.end(), inactivecells.begin(), inactivecells.end());

		if (gameObjects.size() > 0)
		{
			/// collision detected
			CreateCellFailure *failure = new CreateCellFailure();
			failure->requestId = requestId;
			failure->endpoint = currentPlayer.getEndpoint();
			failure->errorCode = CreateCellErrorCode::SpotAlreadyTaken;
			NETWORKMANAGER->send(failure);
			LOG_INFO("CreateCellFailure SpotAlreadyTaken sent");

			LOG_ERROR("Cell could not be created because a gameobject is already at this spot");
			return false;
		}
	}

	// Calculate static Effects on Cell
	auto environment = GAMECONTEXT->getEnvironment().findInRadiusOf(cell.getPosition(), cell.getRadius());
	for (auto it = environment.begin(); it != environment.end(); ++it)
	{
		auto staticModifier = dynamic_cast<StaticModificatorServer*>(*it);
		if (staticModifier)
		{
			cell.addStaticModificator(staticModifier);
		}
	}

	GAMECONTEXT->getInactiveCells().createGameObject(&cell);

	(*EVENT_MGR) += new BuildingEvent(time, cell);

	EventManager & em = (*EVENT_MGR);
		
	CreateCellSuccess * success = new CreateCellSuccess();
	success->requestId = requestId;
	success->endpoint = currentPlayer.getEndpoint();
	success->cellId = cell.getId();
	success->position[0] = cell.getPosition()[0];
	success->position[1] = cell.getPosition()[1];
	success->angle = cell.getAngle();
	NETWORKMANAGER->send(success);
	LOG_INFO("CreateCellSuccess sent");

	
	CellNew * cellNew = new CellNew();
	cellNew->playerId = currentPlayer.getId();
	cellNew->cellId = cell.getId();
	cellNew->position[0] = cell.getPosition()[0];
	cellNew->position[1] = cell.getPosition()[1];
	cellNew->angle = cell.getAngle();
	cellNew->type = type;

	using boost::asio::ip::udp;

	vector<udp::endpoint> endpointArr;

	for (auto it = GAMECONTEXT->getPlayerMap().begin(); it != GAMECONTEXT->getPlayerMap().end(); ++it)
	{
		if (it->second->getId() != currentPlayer.getId())
		{
			endpointArr.push_back(it->second->getEndpoint());
		}
	}

	NETWORKMANAGER->sendTo<CellNew>(cellNew, endpointArr);
	LOG_INFO("CellNew sent");

	return true;
}

bool EventCreator::createAttackEvent(const double time, bool isAttacker, const PlayerServer & currentPlayer, CellServer & currentCell)
{
	if (isAttacker && (!currentCell.getIsComplete() || currentCell.getType() != CellServer::STANDARDCELL))
	{
		LOG_INFO("No attack is performed");
		return false;
	}

	/** 
		* search for all cells that would be in its attack radius
		* reversely all the other cell that are attackers have the current cell in its attack radius
		*/
	const vector<GameObject *> & gameObjects =
		GAMECONTEXT->getActiveCells().findInRadiusOf(currentCell.getPosition(), currentCell.getRadius() + CONFIG_FLOAT1("data.cell.standardcell.attackradius"));
					
	ci::Vec3f attacker;
	ci::Vec3f victim;
	/// those assignments are true for all iterations
	if (isAttacker)
	{
		attacker = currentCell.getPosition();
	}
	else
	{
		victim = currentCell.getPosition();
	}
	for (auto it = gameObjects.begin(); it != gameObjects.end(); ++it)
	{
		CellServer * actualCell = dynamic_cast<CellServer *>(*it);
		if (actualCell->getOwner() != &currentPlayer)
		{
			if (actualCell != 0 && actualCell->getIsComplete())
			{
				/// those assignments depend on the iterator
				if (isAttacker)
				{
					victim = actualCell->getPosition();
				}
				else
				{
					if (actualCell->getType() != CellServer::STANDARDCELL) continue;
					attacker = actualCell->getPosition();
				}

				ci::Vec3f attackerDir(Vec3f::xAxis());
				attackerDir.rotate(Vec3f::zAxis(), actualCell->getAngle());
				attackerDir.normalize();

				ci::Vec3f attacker2VictimDir;
				attacker2VictimDir = victim - attacker;
				attacker2VictimDir.normalize();


				float attackAngle = acosf(attackerDir.dot(attacker2VictimDir)) * 180.f / float(M_PI);
				float distanceDropOffDegree = CONFIG_FLOAT1("data.cell.standardcell.distanceDropOffDegree");
				float attackPower = min<float>(max<float>((distanceDropOffDegree - attackAngle) / distanceDropOffDegree, 0.f), 1.f);

				float damage = CONFIG_FLOAT1("data.cell.standardcell.damage") * attackPower;

				if (damage > 0.f)
				{
					CellServer * attackerCell = isAttacker ? &currentCell : actualCell;
					CellServer * victimCell = isAttacker ? actualCell : &currentCell;

					// Modify damage when cells within a static modifier

					auto staticAttackerModifier = attackerCell->getStaticModificator();
					for (auto it = staticAttackerModifier.begin(); it != staticAttackerModifier.end(); ++it)
					{
						switch((*it)->getType())
						{
						case StaticModificator::NUTRIENTSOIL:
							damage *= (1 + ci::randFloat());
							break;
						default:
							break;
						}
					}

					auto staticVictimModifier = victimCell->getStaticModificator();
					for (auto it = staticVictimModifier.begin(); it != staticVictimModifier.end(); ++it)
					{
						switch((*it)->getType())
						{
						case StaticModificator::RADIOACTIVITY:
							damage *= (1 + ci::randFloat());
							break;
						default:
							break;
						}
					}

					(*EVENT_MGR) += new AttackEvent(time, attackerCell->getId(), victimCell->getId(), damage);
				}
				/// attack message is sent in event
			}
		}
	}
	return true;
}

EventCreator::EventCreator() { }