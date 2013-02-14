#include "CellDieEvent.h"
#include "EventManager.h"
#include "../game/CellServer.h"
#include "../game/PlayerServer.h"
#include "../game/GameContext.h"
#include "../../common/GameObjectContainer.h"
#include "../../common/network/NetworkManager.h"
#include "../../common/network/messages/game/ingame/cell/combat/CellAttack.h"
#include "../../common/network/messages/game/ingame/cell/combat/CellDie.h"
#include "../../common/network/messages/game/outgame/GameOver.h"

CellDieEvent::CellDieEvent(double startTime, unsigned int cellId) :
	cellId(cellId),
	GameEvent(startTime, CONFIG_FLOAT1("data.event.celldie.time"))
{
	
}

void CellDieEvent::trigger()
{
	auto tempCell = GAMECONTEXT->getActiveCells().find(cellId);
	if (!tempCell) tempCell = GAMECONTEXT->getInactiveCells().find(cellId);

	if (tempCell != nullptr)
	{
		auto cell = dynamic_cast<CellServer *>(tempCell);

		if (cell != nullptr)
		{
			for (auto it = cell->getChildren().begin(); it != cell->getChildren().end(); ++it)
			{
				(*EVENT_MGR) += new CellDieEvent(getDeadTime(), (*it)->getId());
			}

			CellDie * die = new CellDie();
			die->cellId = cell->getId();

			NETWORKMANAGER->sendTo<CellDie>(die, NETWORKMANAGER->getConnectionEndpoints());
			LOG_INFO("CellDie sent");

			GAMECONTEXT->getActiveCells().removeGameObject(cell->getId());
			GAMECONTEXT->getInactiveCells().removeGameObject(cell->getId());
		}
	}
}