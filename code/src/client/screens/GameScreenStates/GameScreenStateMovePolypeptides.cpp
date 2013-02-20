#include "GameScreenStateMovePolypeptides.h"
#include "GameScreenStateInMenu.h"
#include "GameScreenStateNeutral.h"
#include "../../../common/network/messages/game/ingame/polypeptide/travel/MovePolypeptideRequest.h"
#include "../../managers/GameManager.h"

GameScreenStateMovePolypeptides::GameScreenStateMovePolypeptides(GameScreen * screen):
	GameScreenState(screen),
	neutralState(screen)
{
}

bool GameScreenStateMovePolypeptides::touchClick(TouchWay touchWay)
{
	auto cellsPicked = screen->getCellsPicked(touchWay.getCurrentPos());

	if (cellsPicked.size() > 0)
	{
		if (screen->getSelectedPolypeptides().getSize() != 0)
		{
			auto fromCell = screen->getSelectedPolypeptides().begin()->second->getOwner();
			auto toCell = cellsPicked[0];

			if (fromCell != nullptr && fromCell != toCell)
			{
				MovePolypeptideRequest * request = new MovePolypeptideRequest();
				request->fromCellId = fromCell->getId();
				request->toCellId = toCell->getId();
				request->amount = screen->getSelectedPolypeptides().getSize();
				request->endpoint = GAME_MGR->getServerEndpoint();
				NETWORK_MGR->registerMovePolypeptideRequest(request, 
					fromCell,
					toCell,
					screen->getSelectedPolypeptides().getSize());
				NETWORK_MGR->send(request);
				LOG_INFO("MovePolypeptideRequest sent");
			}
		}
	}
	
	screen->getSelectedPolypeptides().clear();
	screen->switchToState(new GameScreenStateNeutral(screen));

	return true;
}

void GameScreenStateMovePolypeptides::touchMoved(const TouchWay & touchWay)
{
	neutralState.touchMoved(touchWay);
}