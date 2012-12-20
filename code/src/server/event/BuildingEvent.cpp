#include "BuildingEvent.h"

#include "../../common/ConfigurationDataHandler.h"

BuildingEvent::BuildingEvent(double startTime, NetworkManager & manager, CellServer & cell, const vector<PlayerServer *> & players) :
	manager(manager),
	cell(cell),
	players(players),
	GameEvent(startTime, CONFIG_INT1("event.building.time"))
	{ }

void BuildingEvent::trigger()
{
	cell.completeCell();
	PlayerServer * current = 0;
	for (vector<PlayerServer *>::const_iterator it = players.begin(); it != players.end(); ++it)
	{
		CreateCellComplete *complete = new CreateCellComplete();
		complete->cellId = cell.getId();
		complete->endpoint = (*it)->getEndpoint();
		manager.send(complete);

		if ((*it)->getId() == cell.getOwner()->getId())
		{
			current = *it;
		}
	}
	EventCreator::getInstance()->createAttackEvent(m_dDeadTime, true, *current, cell);
}