#include "BuildingEvent.h"

#include "../../common/ConfigurationDataHandler.h"

BuildingEvent::BuildingEvent(double startTime, NetworkManager & manager, CellServer & cell, const vector<PlayerServer *> & players) :
	manager(manager),
	cell(cell),
	players(players),
	GameEvent(startTime, CONFIG_FLOAT1("data.event.build.time"))
	{ }

void BuildingEvent::trigger()
{
	cell.completeCell();
	PlayerServer * current = 0;

	CreateCellComplete * complete = new CreateCellComplete();
	complete->cellId = cell.getId();

	using boost::asio::ip::udp;
	vector<udp::endpoint> endpointArr;

	for (vector<PlayerServer *>::const_iterator it = players.begin(); it != players.end(); ++it)
	{
		endpointArr.push_back((*it)->getEndpoint());
		if ((*it)->getId() == cell.getOwner()->getId())
		{
			current = *it;
		}
	}
	manager.sendTo<CreateCellComplete>(complete, endpointArr);

	EventCreator::getInstance()->createAttackEvent(m_dDeadTime, true, *current, cell);
}