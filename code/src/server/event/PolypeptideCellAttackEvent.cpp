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
		auto polypeptide = attackedCell->getPolypeptides().find(polypeptideId)->second;
		if (polypeptide != nullptr && polypeptide->getState() == Polypeptide::CELLFIGHT)
		{
			attackedCell->decreaseHealthPointsBy(damage);

			PolypeptideCellAttack * polypeptideCellAttack = new PolypeptideCellAttack;
			polypeptideCellAttack->polypeptideId = polypeptideId;
			polypeptideCellAttack->cellId = attackedCellId;
			polypeptideCellAttack->damage = damage;
			NETWORKMANAGER->sendTo<PolypeptideCellAttack>(polypeptideCellAttack, NETWORKMANAGER->getConnectionEndpoints());

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