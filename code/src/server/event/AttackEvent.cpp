#include "AttackEvent.h"
#include "EventManager.h"
#include "../game/CellServer.h"
#include "../game/PlayerServer.h"
#include "../game/GameContext.h"
#include "../../common/GameObjectContainer.h"
#include "../../common/network/NetworkManager.h"
#include "../../common/network/messages/game/ingame/cell/combat/CellAttack.h"
#include "../../common/network/messages/game/ingame/cell/combat/CellDie.h"

AttackEvent::AttackEvent(double startTime, NetworkManager & manager, CellServer & attacker, CellServer & victim, float damage) :
	manager(manager),
	attacker(attacker),
	victim(victim),
	damage(damage),
	GameEvent(startTime, CONFIG_INT1("data.event.attack.time"))
{ }

void AttackEvent::trigger()
{
	victim.decreaseHealthPointsBy(damage);
	CellAttack * attack = new CellAttack();
	attack->attackerCellId = attacker.getId();
	attack->attackedCellId = victim.getId();
	attack->damage = damage;
		
	using boost::asio::ip::udp;
	vector<udp::endpoint> endpointArr;

	for (auto it = GAMECONTEXT->getPlayerMap().begin(); it != GAMECONTEXT->getPlayerMap().end(); ++it)
	{
		endpointArr.push_back(it->second->getEndpoint());
	}

	manager.sendTo<CellAttack>(attack, endpointArr);

	if (victim.getHealthPoints() < 0.f)
	{
		CellDie * die = new CellDie();
		die->cellId = victim.getId();
		PlayerServer * player = 0;
		for (auto it = GAMECONTEXT->getPlayerMap().begin(); it != GAMECONTEXT->getPlayerMap().end(); ++it)
		{
			if (it->second->getId() == victim.getOwner()->getId()) 
			{
				player = it->second;
			}
		}
			
		manager.sendTo<CellDie>(die, endpointArr);

		GAMECONTEXT->getActiveCells().removeGameObject(victim.getId());
	}
	else
	{
		(*EVENT_MGR) += new AttackEvent(this->m_dDeadTime, manager, attacker, victim, damage);
	}
}