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

AttackEvent::AttackEvent(double startTime, CellServer & attacker, CellServer & victim, float damage) :
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

	NETWORKMANAGER->sendTo<CellAttack>(attack, endpointArr);

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
			
		NETWORKMANAGER->sendTo<CellDie>(die, endpointArr);
		
		if (victim.getType() == CellServer::STEMCELL)
		{
			GameOver * gameOver = new GameOver();
			gameOver->playerId = player->getId();

			NETWORKMANAGER->sendTo<GameOver>(gameOver, endpointArr);
			
			player->kill();
		}

		GAMECONTEXT->getActiveCells().removeGameObject(victim.getId());
	}
	else
	{
		(*EVENT_MGR) += new AttackEvent(this->m_dDeadTime, attacker, victim, damage);
	}
}