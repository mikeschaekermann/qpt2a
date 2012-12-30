#pragma once

#include "Event.h"
#include "../game/CellServer.h"
#include "../game/PlayerServer.h"
#include "../../common/network/NetworkManager.h"
#include "../../common/network/messages/game/ingame/cell/combat/CellAttack.h"
#include "../../common/network/messages/game/ingame/cell/combat/CellDie.h"
#include "../event/EventQueue.h"


class AttackEvent : public GameEvent
{
public:
	AttackEvent(double startTime, NetworkManager & manager, EventQueue & queue, GameObjectContainer & gameObjectContainer, CellServer & attacker, CellServer & victim, float damage, vector<PlayerServer *> & players) :
	  manager(manager),
	  queue(queue),
	  gameObjectContainer(gameObjectContainer),
	  attacker(attacker),
	  victim(victim),
	  damage(damage),
	  players(players),
	  GameEvent(startTime, CONFIG_INT1("data.event.attack.time"))
	{ }

	virtual void trigger()
	{
		victim.decreaseHealthPointsBy(damage);
		CellAttack * attack = new CellAttack();
		attack->attackerCellId = attacker.getId();
		attack->attackedCellId = victim.getId();
		attack->damage = damage;
		for (auto it = players.begin(); it != players.end(); ++it)
		{
			attack->endpoint = (*it)->getEndpoint();
			manager.send(attack);
		}

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
				die->endpoint = (*it)->getEndpoint();
				manager.send(die);
			}
			gameObjectContainer.removeGameObject(victim.getId());
		}
		else
		{
			AttackEvent * ae = new AttackEvent(this->m_dDeadTime, manager, queue, gameObjectContainer, attacker, victim, damage, players);
			queue.addEvent(ae);
		}
	}
private:
	NetworkManager & manager;
	EventQueue & queue;
	GameObjectContainer & gameObjectContainer;
	CellServer & attacker;
	CellServer & victim;
	float damage;
	vector<PlayerServer *> & players;
};