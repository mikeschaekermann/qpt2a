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
		CellClient * fromCell = nullptr;
		auto picked = GAME_SCR.getCellsPicked(touchWay.getCurrentPos());
		if (GAME_SCR.getSelectedPolypeptides().begin() != GAME_SCR.getSelectedPolypeptides().end())
		{
			for (auto it = picked.begin(); it != picked.end(); ++it)
			{
				if ((*it)->getId() == GAME_SCR.getSelectedPolypeptides().begin()->second->getOwner()->getId())
				{
					fromCell = *it;
					break;
				}
			}
			if (fromCell != nullptr && fromCell != cellsPicked[0])
			{
				MovePolypeptideRequest * request = new MovePolypeptideRequest();
				NETWORK_MGR->registerMovePolypeptideRequest(request, 
					fromCell,
					cellsPicked[0],
					GAME_SCR.getSelectedPolypeptides().getSize());
				NETWORK_MGR->send(request);
				LOG_INFO("MovePolypeptideRequest sent");
			}
			GAME_SCR.getSelectedPolypeptides().clear();
		}
	}

	screen->switchToState(new GameScreenStateNeutral(screen));

	return true;
}

void GameScreenStateMovePolypeptides::touchMoved(const TouchWay & touchWay)
{
	neutralState.touchMoved(touchWay);
}