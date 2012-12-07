#pragma once

#include <vector>

#include "Event.h"

#include "../game/EventCreator.h"
#include "../game/Cell.h"
#include "../game/Player.h"

#include "../../common/network/NetworkManager.h"
#include "../../common/network/messages/game/ingame/cell/creation/CreateCellComplete.h"

#define BUILDING_EVENT_DURATION 30.f

class BuildingEvent : public Event
{
public:
	BuildingEvent(double startTime, NetworkManager& manager, Cell& cell, const vector<Player&>& players) :
	  m_rManager(manager),
	  m_rCell(cell),
	  m_rPlayers(players),
	  Event(startTime, BUILDING_EVENT_DURATION)
	  { }

	virtual void trigger()
	{
		m_rCell.completeCell();
		Player* current = 0;
		for (vector<Player&>::const_iterator it = m_rPlayers.begin(); it != m_rPlayers.end(); ++it)
		{
			CreateCellComplete complete;
			complete.cellId = m_rCell.getId();
			complete.endpoint = it->getEndpoint();
			m_rManager.send(complete);

			if (it->getConstPopulation().find(m_rCell.getId()))
			{
				current = &(*it);
			}
		}
		EventCreator::getInstance()->createAttackEvent(m_dDeadTime, true, *current, m_rCell);
	}
private:
	NetworkManager& m_rManager;
	Cell& m_rCell;
	const vector<Player&>& m_rPlayers;
};