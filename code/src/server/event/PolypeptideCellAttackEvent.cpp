#include "PolypeptideCellAttackEvent.h"
#include "CellDieEvent.h"
#include "EventManager.h"
#include "../game/CellServer.h"
#include "../game/PlayerServer.h"
#include "../game/GameContext.h"
#include "../../common/GameObjectContainer.h"
#include "../../common/network/NetworkManager.h"
#include "../../common/network/messages/game/ingame/polypeptide/combat/PolypeptideCellAttack.h"
#include "../../common/network/messages/game/outgame/GameOver.h"
#include <cinder/Rand.h>

PolypeptideCellAttackEvent::PolypeptideCellAttackEvent(double startTime, unsigned int attackerCellId, unsigned int attackedCellId, unsigned int polypeptideId, float damage) :
	attackerCellId(attackerCellId),
	attackedCellId(attackedCellId),
	polypeptideId(polypeptideId),
	damage(damage),
	GameEvent(startTime, CONFIG_FLOAT("data.event.polypeptidecellattack.time"))
{ }

void PolypeptideCellAttackEvent::trigger()
{
	auto attackerCell = dynamic_cast<CellServer *>(GAMECONTEXT->getActiveCells().find(attackerCellId));
	auto attackedCell = dynamic_cast<CellServer *>(GAMECONTEXT->getActiveCells().find(attackedCellId));
	if (attackerCell != nullptr && attackedCell != nullptr)
	{
		auto & polypeptideIt = attackerCell->getPolypeptides().find(polypeptideId);
		if (polypeptideIt != attackerCell->getPolypeptides().end())
		{
			auto & polypeptide = polypeptideIt->second;
			if (polypeptide != nullptr && polypeptide->getState() == Polypeptide::CELLFIGHT)
			{
				attackedCell->decreaseHealthPointsBy(damage);

				vector<ConnectionEndpoint> players;
				players.push_back(*(GAMECONTEXT->getPlayer(attackerCell->getOwner()->getId())));
				players.push_back(*(GAMECONTEXT->getPlayer(attackedCell->getOwner()->getId())));

				PolypeptideCellAttack * polypeptideCellAttack = new PolypeptideCellAttack;
				polypeptideCellAttack->attackerCellId = attackerCell->getId();
				polypeptideCellAttack->attackedCellId = attackedCell->getId();
				polypeptideCellAttack->polypeptideId = polypeptideId;
				polypeptideCellAttack->damage = ci::randFloat(0.5f, 2.0f) * damage;
				NETWORKMANAGER->sendTo<PolypeptideCellAttack>(polypeptideCellAttack, players);

				if (attackedCell->getHealthPoints() < 0.f)
				{
					(*EVENT_MGR) += new CellDieEvent(getDeadTime() - CONFIG_FLOAT("data.event.celldie.time"), attackedCell->getId());

					if (attackedCell->getType() == CellServer::STEMCELL)
					{
						PlayerServer * player = GAMECONTEXT->getPlayer(attackedCell->getOwner()->getId());

						GameOver * gameOver = new GameOver();
						gameOver->playerId = player->getId();

						NETWORKMANAGER->sendTo<GameOver>(gameOver, NETWORKMANAGER->getConnectionEndpoints());
						LOG_INFO("GameOver sent");
			
						player->kill();
					}
				}
				else
				{
					(*EVENT_MGR) += new PolypeptideCellAttackEvent(this->m_dDeadTime, attackerCellId, attackedCellId, polypeptideId, damage);
				}
			}
		}
	}
}