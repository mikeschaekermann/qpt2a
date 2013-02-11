#include "BuildingEvent.h"

#include "../../common/Config.h"
#include "../../common/ConfigurationDataHandler.h"

#include "../../common/network/NetworkManager.h"
#include "../../common/network/messages/game/ingame/cell/creation/CreateCellComplete.h"

#include "../game/CellServer.h"
#include "../game/PlayerServer.h"
#include "../game/GameContext.h"
#include "EventCreator.h"

BuildingEvent::BuildingEvent(double startTime, NetworkManager & manager, CellServer & cell) :
	manager(manager),
	cell(cell),
	GameEvent(startTime, CONFIG_FLOAT1("data.event.build.time"))
	{ }

void BuildingEvent::trigger()
{
	cell.completeCell();

	GAMECONTEXT->getActiveCells().createGameObject(GAMECONTEXT->getInactiveCells().find(cell.getId()));
	GAMECONTEXT->getInactiveCells().removeGameObject(cell.getId(), false);

	LOG_INFO(stringify(ostringstream() << "Cell with the id " << cell.getId() << " is finished"));

	PlayerServer * current = 0;

	CreateCellComplete * complete = new CreateCellComplete();
	complete->cellId = cell.getId();

	using boost::asio::ip::udp;
	vector<udp::endpoint> endpointArr;

	for (auto it = GAMECONTEXT->getPlayerMap().begin(); it != GAMECONTEXT->getPlayerMap().end(); ++it)
	{
		endpointArr.push_back(it->second->getEndpoint());
		if (it->second->getId() == cell.getOwner()->getId())
		{
			current = it->second;
		}
	}
	manager.sendTo<CreateCellComplete>(complete, endpointArr);

	EVENT_CRTR->createAttackEvent(m_dDeadTime, true, *current, cell);
}