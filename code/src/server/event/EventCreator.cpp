#include "EventCreator.h"

#include "../../common/Config.h"

#include "../../common/network/NetworkManager.h"
#include "../../common/network/messages/game/ingame/cell/creation/CreateCellFailure.h"
#include "../../common/network/messages/game/ingame/cell/creation/CellNew.h"
#include "../../common/network/messages/game/ingame/cell/creation/CreateCellSuccess.h"

#include "../../common/ConfigurationDataHandler.h"

#include "../../common/GameObjectContainer.h"

#include "../game/PlayerServer.h"
#include "../game/CellServer.h"
#include "../game/GameContext.h"
#include "../game/PolypeptideServer.h"

#include "../environment/StaticModificatorServer.h"
#include "../environment/BarrierServer.h"

#include "cinder/Rand.h"

#include "GameEvent.h"
#include "EventManager.h"
#include "AttackEvent.h"
#include "BuildingEvent.h"
#include "PolypeptideFightEvent.h"
#include "PolypeptideCellAttackEvent.h"

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

bool EventCreator::createBuildEvent(const double time, const unsigned int requestId, const int type, const float angle, PlayerServer & currentPlayer, CellServer & parentCell, CellServer & cell)
{
	// Check build location not outside the world
	if (!checkInWorldRadius(requestId, cell, currentPlayer)) return false;

	// Check build location not intersecting with other cells or structures
	if (!checkCollision(requestId, cell, currentPlayer)) return false;
	
	// Calculate static Effects on Cell
	calculateStaticEffects(cell);	

	// Add the cell
	cell.addParent(&parentCell);
	parentCell.addChild(&cell);
	GAMECONTEXT->getInactiveCells().createGameObject(&cell);

	// Quere the build event
	(*EVENT_MGR) += new BuildingEvent(time, cell.getId());

	// Let the other cells attack this cell
	//createAttackEvent(time, false, cell);
	
	// Inform the clients of the new cell
	sendCellCreationMessages(requestId, currentPlayer, cell, type);

	return true;
}

bool EventCreator::createAttackEvent(const double time, bool isAttacker, CellServer & currentCell)
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
		GAMECONTEXT->getActiveCells().findInRadiusOf(currentCell.getPosition(), currentCell.getRadius() + CONFIG_FLOAT("data.cell.standardcell.attackradius"));

	for (auto it = gameObjects.begin(); it != gameObjects.end(); ++it)
	{
		CellServer * victimCell = isAttacker ? dynamic_cast<CellServer *>(*it) : &currentCell;
		CellServer * attackerCell = isAttacker ? &currentCell : dynamic_cast<CellServer *>(*it);
		GAMECONTEXT->getAttackRelations().addRelation(*attackerCell, *victimCell);
		if (attackerCell->getType() == CellServer::STANDARDCELL)
		{
			if (victimCell->getOwner()->getId() != attackerCell->getOwner()->getId())
			{
				float damage = calculateDamage(attackerCell, victimCell);

				if (damage > 0.5f)
				{
					// Modify damage when cells within a static modifier
					damage *= getAttackerMultiplier(attackerCell);
					damage *= getVictimMultiplier(victimCell);

					(*EVENT_MGR) += new AttackEvent(time, attackerCell->getId(), victimCell->getId(), damage);
				}
				else
				{
					LOG_INFO("Negative damage not valid");
				}
			}
		}
	}
	GAMECONTEXT->getAttackRelations().update();

	return true;
}

GameEvent * EventCreator::createPolypeptideFightEvent(double startTime, unsigned int cellId1, unsigned int cellId2, unsigned int polypeptideId1, unsigned int polypeptideId2)
{
	PolypeptideFightEvent * polypeptideFightEvent = new PolypeptideFightEvent(startTime, cellId1, cellId2, polypeptideId1, polypeptideId2);
	(*EVENT_MGR) += polypeptideFightEvent;
	return polypeptideFightEvent;
}

GameEvent * EventCreator::createPolypeptideCellAttackEvent(double startTime, unsigned int attackerCellId, unsigned int attackedCellId, unsigned int polypeptideId)
{
	PolypeptideCellAttackEvent * polypeptideCellAttackEvent = new PolypeptideCellAttackEvent(startTime, attackerCellId, attackedCellId, polypeptideId, CONFIG_FLOAT("data.polypeptide.damage"));
	(*EVENT_MGR) += polypeptideCellAttackEvent;
	return polypeptideCellAttackEvent;
}

void EventCreator::sendCellCreationMessages(unsigned int requestId, PlayerServer & currentPlayer, CellServer & cell, const int type)
{
	// Success message is to inform the requesting player
	CreateCellSuccess * success = new CreateCellSuccess();
	success->requestId = requestId;
	success->endpoint = currentPlayer.getEndpoint();
	success->cellId = cell.getId();
	success->position[0] = cell.getPosition()[0];
	success->position[1] = cell.getPosition()[1];
	success->angle = cell.getAngle();
	NETWORKMANAGER->send(success);
	LOG_INFO("CreateCellSuccess sent");
	
	// CellNew message is to inform all the players of the new cell
	CellNew * cellNew = new CellNew();
	cellNew->playerId = currentPlayer.getId();
	cellNew->cellId = cell.getId();
	cellNew->position[0] = cell.getPosition()[0];
	cellNew->position[1] = cell.getPosition()[1];
	cellNew->angle = cell.getAngle();
	cellNew->type = type;

	NETWORKMANAGER->sendTo<CellNew>(cellNew, NETWORKMANAGER->getConnectionEndpoints());
	LOG_INFO("CellNew sent");
}

void EventCreator::calculateStaticEffects(CellServer & cell)
{
	auto environment = GAMECONTEXT->getEnvironment().findInRadiusOf(cell.getPosition(), cell.getRadius());
	for (auto it = environment.begin(); it != environment.end(); ++it)
	{
		auto staticModifier = dynamic_cast<StaticModificatorServer*>(*it);
		if (staticModifier)
		{
			cell.addStaticModificator(staticModifier);
		}
	}
}

bool EventCreator::checkInWorldRadius(unsigned int requestId, CellServer & cell, const PlayerServer & player)
{
	// The cell has to be in the world to be placed!
	if (!isInRadiusOf<float>(cell.getPosition(), cell.getRadius(), Vec3f::zero(), CONFIG_FLOAT("data.world.radius")))
	{
		CreateCellFailure *failure = new CreateCellFailure();
		failure->endpoint = player.getEndpoint();
		failure->requestId = requestId;
		failure->errorCode = CreateCellErrorCode::OutsideGameArea;
		NETWORKMANAGER->send(failure);
		LOG_INFO("CreateCellFailure OutsideGameArea sent");

		LOG_ERROR("Cell could not be created because it is not in the game area");
		return false;
	}
	return true;
}

bool EventCreator::checkCollision(unsigned int requestId, CellServer & cell, const PlayerServer & player)
{
	for (auto it = GAMECONTEXT->getPlayerMap().begin(); it != GAMECONTEXT->getPlayerMap().end(); ++it)
	{
		const vector<GameObject *> & activeCells = GAMECONTEXT->getActiveCells().findInRadiusOf(cell.getPosition(), cell.getRadius());
		const vector<GameObject *> & inactivecells =  GAMECONTEXT->getInactiveCells().findInRadiusOf(cell.getPosition(), cell.getRadius());
		const vector<GameObject *> & environment =  GAMECONTEXT->getEnvironment().findInRadiusOf(cell.getPosition(), cell.getRadius());


		vector<GameObject *> gameObjects;
		gameObjects.insert(gameObjects.end(), activeCells.begin(), activeCells.end());
		gameObjects.insert(gameObjects.end(), inactivecells.begin(), inactivecells.end());

		for (auto it = environment.begin(); it != environment.end(); ++it)
		{
			auto modifier = dynamic_cast<BarrierServer *>(*it);
			if (modifier)
			{
				gameObjects.push_back(modifier);
			}
		}

		// There are cells colliding with this cell
		if (gameObjects.size() > 0)
		{
			/// collision detected
			CreateCellFailure *failure = new CreateCellFailure();
			failure->requestId = requestId;
			failure->endpoint = player.getEndpoint();
			failure->errorCode = CreateCellErrorCode::SpotAlreadyTaken;
			NETWORKMANAGER->send(failure);
			LOG_INFO("CreateCellFailure SpotAlreadyTaken sent");

			LOG_ERROR("Cell could not be created because a gameobject is already at this spot");
			return false;
		}
	}

	return true;
}

float EventCreator::calculateDamage(CellServer * attacker, CellServer * victim)
{
	float damage = 0.f;

	if (attacker && victim)
	{
		ci::Vec3f attackerDir(cosf(attacker->getAngle()), sinf(attacker->getAngle()), 0.f);
		attackerDir.normalize();

		ci::Vec3f attacker2VictimDir = victim->getPosition() - attacker->getPosition();
		attacker2VictimDir.normalize();

		float maxSpikeLength = CONFIG_FLOAT("data.cell.standardcell.attackradius");
		float distanceDropOffDegree = CONFIG_FLOAT("data.cell.standardcell.distanceDropOffDegree");
		float attackAngle = acosf(attackerDir.dot(attacker2VictimDir)) * 180.f / float(M_PI);
		float spikeLength = maxSpikeLength * min<float>(max<float>((distanceDropOffDegree - attackAngle) / distanceDropOffDegree, 0.f), 1.f);
		float punctureDepth = spikeLength - ((victim->getPosition() - attacker->getPosition()).length() - attacker->getRadius() - victim->getRadius());
		damage = punctureDepth;
	}

	return damage;
}

float EventCreator::getAttackerMultiplier(CellServer * attacker)
{
	float multiplicator = 1.f;

	auto staticAttackerModifier = attacker->getStaticModificator();
	for (auto it = staticAttackerModifier.begin(); it != staticAttackerModifier.end(); ++it)
	{
		switch((*it)->getType())
		{
		case StaticModificator::NUTRIENTSOIL:
			multiplicator *= (1 + ci::randFloat());
			break;
		default:
			break;
		}
	}

	return multiplicator;
}

float EventCreator::getVictimMultiplier(CellServer * victim)
{
	float multiplicator = 1.f;

	auto staticVictimModifier = victim->getStaticModificator();
	for (auto it = staticVictimModifier.begin(); it != staticVictimModifier.end(); ++it)
	{
		switch((*it)->getType())
		{
		case StaticModificator::RADIOACTIVITY:
			multiplicator *= (1 + ci::randFloat());
			break;
		default:
			break;
		}
	}

	return multiplicator;
}

void EventCreator::createPolypeptideCellAttackEvent(double time, unsigned int attackerCellId, unsigned int attackedCellId, vector<unsigned int> & polypeptideIds, float damage)
{
	for (unsigned int i = 0; i < polypeptideIds.size(); ++i)
	{
		(*EVENT_MGR) += new PolypeptideCellAttackEvent(time, attackerCellId, attackedCellId, polypeptideIds[i], damage);
	}
}

EventCreator::EventCreator() { }