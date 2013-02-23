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
			auto player = GAMECONTEXT->getPlayer(cell->getOwner()->getId());

			for (auto it = cell->getPolypeptides().begin(); it != cell->getPolypeptides().end(); ++it)
			{
				LOG_INFO("PolypeptideDie sent because cell died");
				PolypeptideDie * message = new PolypeptideDie();
				message->cellId = cell->getId();
				message->polypeptideId = it->second->getId();
				message->endpoint = player->getEndpoint();
				NETWORKMANAGER->send(message);

				delete it->second;
				--(POLYCAPACITY(player->getId())->NumberOfPolypeptides);
			}

			for (auto it = cell->getChildren().begin(); it != cell->getChildren().end(); ++it)
			{
				(*EVENT_MGR) += new CellDieEvent(getDeadTime(), (*it)->getId());
			}

			if (cell->getType() == CellServer::STANDARDCELL)
			{
				--(POLYCAPACITY(player->getId())->NumberOfStandardCells);
			}
			else if (cell->getType() == CellServer::BONECELL)
			{
				--(POLYCAPACITY(player->getId())->NumberOfBoneCells);
			}
			
			bool isStemCell = cell->getType() == CellServer::STEMCELL;
			bool hasTooManyPolypeptides = POLYCAPACITY(player->getId())->getRemainingNumberOfPolypeptidesAllowed() < 0;

			if (!isStemCell && hasTooManyPolypeptides)
			{
				unsigned int nrOfPolys = -POLYCAPACITY(player->getId())->getRemainingNumberOfPolypeptidesAllowed();

				queue<CellServer *> q;
				q.push(&(player->getStemCell()));

				unsigned int i = 0;
				while (!q.empty())
				{
					auto curCell = q.front();
					q.pop();
					auto polyIt = curCell->getPolypeptides().begin();
					while (i < nrOfPolys && polyIt != curCell->getPolypeptides().end())
					{
						LOG_INFO("PolypeptideDie sent because capacity is low");
						PolypeptideDie * message = new PolypeptideDie();
						message->cellId = curCell->getId();
						message->polypeptideId = polyIt->second->getId();
						message->endpoint = player->getEndpoint();
						NETWORKMANAGER->send(message);

						delete polyIt->second;
						polyIt = curCell->getPolypeptides().erase(polyIt);

						++i;
					}

					if (i == nrOfPolys) break;

					for (auto it = curCell->getChildren().begin(); it != curCell->getChildren().end(); ++it)
					{
						q.push(dynamic_cast<CellServer *>(*it));
					}
				}

				POLYCAPACITY(player->getId())->NumberOfPolypeptides -= nrOfPolys;
			}

			CellDie * die = new CellDie();
			die->cellId = cell->getId();

			NETWORKMANAGER->sendTo<CellDie>(die, NETWORKMANAGER->getConnectionEndpoints());
			LOG_INFO("CellDie sent");
			
			if (!isStemCell)
			{
				GAMECONTEXT->getAttackRelations().removeRelationsFor(*cell);
				GAMECONTEXT->getAttackRelations().updateRelationsFor(*cell);
				GAMECONTEXT->getActiveCells().removeGameObject(cell->getId());
			}
		}
	}
}