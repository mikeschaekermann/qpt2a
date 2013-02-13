#include "AttackEvent.h"
#include "CellDieEvent.h"
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
	attackerId(attackerId),
	attackedId(attackedId),
	GameEvent(startTime, CONFIG_FLOAT1("data.event.attack.time"))
{ }

void AttackEvent::trigger()
{
	auto attackerO = GAMECONTEXT->getActiveCells().find(attackerId);
	auto attackedO = GAMECONTEXT->getActiveCells().find(attackedId);
	if (attackerO != nullptr && attackedO != nullptr)
	{
		auto attacker = dynamic_cast<CellServer *>(attackerO);
		auto attacked = dynamic_cast<CellServer *>(attackedO);

		attacked->decreaseHealthPointsBy(damage);
		CellAttack * attack = new CellAttack();
		attack->attackerCellId = attacker->getId();
		attack->attackedCellId = attacked->getId();
		attack->damage = damage;

		LOG_INFO(stringify(ostringstream() << "Cell with id: " << attack->attackerCellId << " is attacking cell with id: " << attack->attackedCellId << " width a damage of: " << attack->damage));
		
		using boost::asio::ip::udp;
		vector<udp::endpoint> endpointArr;

		for (auto it = GAMECONTEXT->getPlayerMap().begin(); it != GAMECONTEXT->getPlayerMap().end(); ++it)
		{
			endpointArr.push_back(it->second->getEndpoint());
		}

		NETWORKMANAGER->sendTo<CellAttack>(attack, endpointArr);
		LOG_INFO("CellAttack sent");

		if (attacked->getHealthPoints() < 0.f)
		{
			(*EVENT_MGR) += new CellDieEvent(getDeadTime() - CONFIG_FLOAT1("data.event.celldie.time"), attacked->getId());

			if (attacked->getType() == CellServer::STEMCELL)
			{
				PlayerServer * player = 0;
				for (auto it = GAMECONTEXT->getPlayerMap().begin(); it != GAMECONTEXT->getPlayerMap().end(); ++it)
				{
					if (it->second->getId() == attacked->getOwner()->getId()) 
					{
						player = it->second;
					}
				}

				GameOver * gameOver = new GameOver();
				gameOver->playerId = player->getId();

				NETWORKMANAGER->sendTo<GameOver>(gameOver, endpointArr);
				LOG_INFO("GameOver sent");
			
				player->kill();
			}
		}
		else
		{
			(*EVENT_MGR) += new AttackEvent(this->m_dDeadTime, attacker->getId(), attacked->getId(), damage);
		}
	}
}