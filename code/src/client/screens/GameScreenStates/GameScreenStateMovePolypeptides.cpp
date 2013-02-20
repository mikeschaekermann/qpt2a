#include "GameScreenStateMovePolypeptides.h"
#include "GameScreenStateInMenu.h"
#include "GameScreenStateNeutral.h"
#include "../../../common/network/messages/game/ingame/polypeptide/travel/MovePolypeptideRequest.h"
#include "../../managers/GameManager.h"

GameScreenStateMovePolypeptides::GameScreenStateMovePolypeptides(GameScreen * screen):
	GameScreenState(screen)
{
}

void GameScreenStateMovePolypeptides::draw2D()
{
	
	glPushAttrib(GL_COLOR);
	{
		glColor4f(ColorA(1.f, 0.f, 0.f, 1.f));
		auto polypeptides = screen->getSelectedPolypeptides();
		unsigned int i = 0;
		for (auto it = polypeptides.begin(); it != polypeptides.end(); ++it)
		{
			float angle = 360.f / polypeptides.getSize();
			float distance = 15.f + polypeptides.getSize();
			Vec2f pos = position + Vec2f(cosf(toRadians(angle * i)) * distance, sinf(toRadians(angle * i)) * distance);
			gl::drawSolidCircle(position + Vec2f(cosf(toRadians(angle * i)) * distance, sinf(toRadians(angle * i)) * distance), 5.f);
			++i;
		}
	}
	glPopAttrib();
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
	
	screen->switchToState(new GameScreenStateNeutral(screen));

	return true;
}

bool GameScreenStateMovePolypeptides::touchBegan(const TouchWay & touchWay)
{
	return false;
}

void GameScreenStateMovePolypeptides::touchMoved(const TouchWay & touchWay)
{
	position = touchWay.getCurrentPos();
	GameScreenState::touchMoved(touchWay);
}

bool GameScreenStateMovePolypeptides::mouseMove(MouseEvent event) 
{ 
	position = event.getPos();
	return false; 
}