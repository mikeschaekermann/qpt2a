#pragma once

#include "Event.h"
#include "../game/Cell.h"
#include "../game/Player.h"
#include "../../common/network/NetworkManager.h"
#include "../../common/network/messages/game/ingame/cell/combat/CellAttack.h"
#include "../../common/network/messages/game/ingame/cell/combat/CellDie.h"
#include "../event/EventQueue.h"

#define ATTACK_EVENT_DURATION 30.f

class AttackEvent : public Event
{
public:
	AttackEvent(double startTime, NetworkManager& manager, EventQueue& queue, Cell& attacker, Cell& victim, float damage, vector<Player&>& players) :
	  m_rManager(manager),
	  m_rQueue(queue),
	  m_rAttacker(attacker),
	  m_rVictim(victim),
	  m_fDamage(damage),
	  m_rPlayers(players),
	  Event(startTime, ATTACK_EVENT_DURATION)
	{ }

	virtual void trigger()
	{
		m_rVictim.decreaseHealthPointsBy(m_fDamage);
		CellAttack attack;
		attack.attackerCellId = m_rAttacker.getId();
		attack.attackedCellId = m_rVictim.getId();
		attack.damage = m_fDamage;
		for (vector<Player&>::const_iterator it = m_rPlayers.begin(); it != m_rPlayers.end(); ++it)
		{
			attack.endpoint = it->getEndpoint();
			m_rManager.send(attack);
		}

		if (m_rVictim.getHealthPoints() < 0.f)
		{
			CellDie die;
			die.cellId = m_rVictim.getId();
			Player* player = 0;
			for (vector<Player&>::iterator it = m_rPlayers.begin(); it != m_rPlayers.end(); ++it)
			{
				if (it->getPopulation().find(m_rVictim.getId()) != 0) 
				{
					player = &(*it);
				}
				die.endpoint = it->getEndpoint();
				m_rManager.send(die);
			}
			player->getPopulation().removeCell(m_rVictim.getId());
		}
		else
		{
			AttackEvent* ae = new AttackEvent(this->m_dDeadTime, m_rManager, m_rQueue, m_rAttacker, m_rVictim, m_fDamage, m_rPlayers);
			m_rQueue.addEvent(ae);
		}
	}
private:
	NetworkManager& m_rManager;
	EventQueue& m_rQueue;
	Cell& m_rAttacker;
	Cell& m_rVictim;
	float m_fDamage;
	vector<Player&>& m_rPlayers;
};