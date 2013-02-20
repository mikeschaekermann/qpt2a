#include "CellDieEvent.h"
#include "EventManager.h"
#include "../game/CellServer.h"
#include "../game/PlayerServer.h"
#include "../game/GameContext.h"
#include "../../common/GameObjectContainer.h"
#include "../../common/network/NetworkManager.h"
#include "../../common/network/messages/game/ingame/cell/combat/CellAttack.h"
#include "../../common/network/messages/game/ingame/cell/combat/CellDie.h"
#include "../../common/network/messages/game/ingame/polypeptide/combat/PolypeptideDie.h"
#include "../../common/network/messages/game/outgame/GameOver.h"
#include "../../common/PolypeptideCapacityContainer.h"

#include <queue>

CellDieEvent::CellDieEvent(double startTime, unsigned int cellId) :
	cellId(cellId),
	GameEvent(startTime, CONFIG_FLOAT("data.event.celldie.time"))
{
	
}

void CellDieEvent::trigger()
{
	auto tempCell = GAMECONTEXT->getActiveCells().find(cellId);

	if (tempCell != nullptr)
	{
		auto cell = dynamic_cast<CellServer *>(tempCell);

		if (cell != nullptr)
		{
			for (auto it = cell->getChildren().begin(); it != cell->getChildren().end(); ++it)
			{
				(*EVENT_MGR) += new CellDieEvent(getDeadTime(), (*it)->getId());
			}

			if (cell->getType() == CellServer::STANDARDCELL)
			{
				--(POLYCAPACITY->NumberOfStandardCells);
			}
			else if (cell->getType() == CellServer::BONECELL)
			{
				--(POLYCAPACITY->NumberOfBoneCells);
			}

			if (POLYCAPACITY->getRemainingNumberOfPolypeptidesAllowed() < 0)
			{
				auto player = GAMECONTEXT->getPlayer(cell->getOwner()->getId());
				auto nrOfPolys = -POLYCAPACITY->getRemainingNumberOfPolypeptidesAllowed();

				queue<CellServer *> q;
				q.push(&(player->getStemCell()));

				unsigned int i = 0;
				while (!q.empty())
				{
					auto curCell = q.front();
					for (auto it = curCell->getPolypeptides().begin(); i < nrOfPolys && it != curCell->getPolypeptides().end(); ++i)
					{
						LOG_INFO("PolypeptideDie sent because capacity is low");
						PolypeptideDie * message = new PolypeptideDie();
						message->polypeptideId = it->second->getId();
						message->endpoint = player->getEndpoint();
						NETWORKMANAGER->send(message);

						delete it->second;
						it = curCell->getPolypeptides().erase(it);
					}

					if (i == nrOfPolys) break;

					for (auto it = curCell->getChildren().begin(); it != curCell->getChildren().end(); ++it)
					{
						q.push(dynamic_cast<CellServer *>(*it));
					}
				}

				POLYCAPACITY->NumberOfPolypeptides -= nrOfPolys;
			}

			CellDie * die = new CellDie();
			die->cellId = cell->getId();

			NETWORKMANAGER->sendTo<CellDie>(die, NETWORKMANAGER->getConnectionEndpoints());
			LOG_INFO("CellDie sent");
			
			GAMECONTEXT->getAttackRelations().removeRelationsWith(*cell);
			GAMECONTEXT->getAttackRelations().update();
			GAMECONTEXT->getActiveCells().removeGameObject(cell->getId());
		}
	}
}