#include "AttackEvent.h"
#include "EventManager.h"
#include "../game/CellServer.h"
#include "../game/PlayerServer.h"
#include "../../common/GameObjectContainer.h"
#include "../../common/network/NetworkManager.h"
#include "../../common/network/messages/game/ingame/cell/combat/CellAttack.h"
#include "../../common/network/messages/game/ingame/cell/combat/CellDie.h"

AttackEvent::AttackEvent(double startTime, NetworkManager & manager, GameObjectContainer & gameObjectContainer, CellServer & attacker, CellServer & victim, float damage, vector<PlayerServer *> & players) :
	manager(manager),
	gameObjectContainer(gameObjectContainer),
	attacker(attacker),
	victim(victim),
	damage(damage),
	players(players),
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

	for (auto it = players.begin(); it != players.end(); ++it)
	{
		endpointArr.push_back((*it)->getEndpoint());
	}

	manager.sendTo<CellAttack>(attack, endpointArr);

	if (victim.getHealthPoints() < 0.f)
	{
		CellDie * die = new CellDie();
		die->cellId = victim.getId();
		PlayerServer * player = 0;
		for (auto it = players.begin(); it != players.end(); ++it)
		{
			if ((*it)->getId() == victim.getOwner()->getId()) 
			{
				player = *it;
			}
		}
			
		manager.sendTo<CellDie>(die, endpointArr);

		gameObjectContainer.removeGameObject(victim.getId());
	}
	else
	{
		(*EVENT_MGR) += new AttackEvent(this->m_dDeadTime, manager, gameObjectContainer, attacker, victim, damage, players);
	}
}