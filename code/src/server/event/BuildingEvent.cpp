#include "BuildingEvent.h"

#include "../../common/Config.h"
#include "../../common/ConfigurationDataHandler.h"

#include "../../common/network/NetworkManager.h"
#include "../../common/network/messages/game/ingame/cell/creation/CreateCellComplete.h"

#include "../game/CellServer.h"
#include "../game/PlayerServer.h"
#include "EventCreator.h"

BuildingEvent::BuildingEvent(double startTime, NetworkManager & manager, CellServer & cell, const vector<PlayerServer *> & players) :
	manager(manager),
	cell(cell),
	players(players),
	GameEvent(startTime, CONFIG_FLOAT1("data.event.build.time"))
	{ }

void BuildingEvent::trigger()
{
	cell.completeCell();
	LOG_INFO(stringify(ostringstream() << "Cell with the id " << cell.getId() << " is finished"));

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

	EVENT_CRTR->createAttackEvent(m_dDeadTime, true, *current, cell);
}