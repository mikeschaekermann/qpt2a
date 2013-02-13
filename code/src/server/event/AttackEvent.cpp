#include "AttackEvent.h"
#include "EventManager.h"
#include "../game/CellServer.h"
#include "../game/PlayerServer.h"
#include "../game/GameContext.h"
#include "../../common/GameObjectContainer.h"
#include "../../common/network/NetworkManager.h"
#include "../../common/network/messages/game/ingame/cell/combat/CellAttack.h"
#include "../../common/network/messages/game/ingame/cell/combat/CellDie.h"
#include "../../common/network/messages/game/outgame/GameOver.h"

AttackEvent::AttackEvent(double startTime, unsigned int attackerId, unsigned int attackedId, float damage) :
	damage(damage),
	GameEvent(startTime, CONFIG_FLOAT1("data.event.attack.time"))
{
	auto attackCell = GAMECONTEXT->getActiveCells().find(attackerId);
	if (attackCell != nullptr)
	{
		auto victimCell = GAMECONTEXT->getActiveCells().find(attackedId);
		if (victimCell != nullptr)
		{
			attacker = dynamic_cast<CellServer *>(attackCell);
			victim = dynamic_cast<CellServer *>(victimCell);
			return;
		}
	}
	attacker = nullptr;
	victim = nullptr;
}

void AttackEvent::trigger()
{
	if (attacker != nullptr && victim != nullptr)
	{
		victim->decreaseHealthPointsBy(damage);
		CellAttack * attack = new CellAttack();
		attack->attackerCellId = attacker->getId();
		attack->attackedCellId = victim->getId();
		attack->damage = damage;

		LOG_INFO(stringify(ostringstream() << "Cell with id: " << attack->attackedCellId << " is attacking cell with id: " << attack->attackedCellId << " width a damage of: " << attack->damage));
		
		using boost::asio::ip::udp;
		vector<udp::endpoint> endpointArr;

		for (auto it = GAMECONTEXT->getPlayerMap().begin(); it != GAMECONTEXT->getPlayerMap().end(); ++it)
		{
			endpointArr.push_back(it->second->getEndpoint());
		}

		NETWORKMANAGER->sendTo<CellAttack>(attack, endpointArr);
		LOG_INFO("CellAttack sent");

		if (victim->getHealthPoints() < 0.f)
		{
			CellDie * die = new CellDie();
			die->cellId = victim->getId();
			PlayerServer * player = 0;
			for (auto it = GAMECONTEXT->getPlayerMap().begin(); it != GAMECONTEXT->getPlayerMap().end(); ++it)
			{
				if (it->second->getId() == victim->getOwner()->getId()) 
				{
					player = it->second;
				}
			}
			
			NETWORKMANAGER->sendTo<CellDie>(die, endpointArr);
			LOG_INFO("CellDie sent");
		
			if (victim->getType() == CellServer::STEMCELL)
			{
				GameOver * gameOver = new GameOver();
				gameOver->playerId = player->getId();

				NETWORKMANAGER->sendTo<GameOver>(gameOver, endpointArr);
				LOG_INFO("GameOver sent");
			
				player->kill();
			}

			GAMECONTEXT->getActiveCells().removeGameObject(victim->getId());
		}
		else
		{
			(*EVENT_MGR) += new AttackEvent(this->m_dDeadTime, attacker->getId(), victim->getId(), damage);
		}
	}
}