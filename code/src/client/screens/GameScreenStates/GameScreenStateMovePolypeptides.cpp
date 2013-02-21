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
		glColor4f(ColorA(
			CONFIG_FLOAT("data.ingamefeedback.pickPolypeptides.movingPolypeptideColor.r"), 
			CONFIG_FLOAT("data.ingamefeedback.pickPolypeptides.movingPolypeptideColor.g"), 
			CONFIG_FLOAT("data.ingamefeedback.pickPolypeptides.movingPolypeptideColor.b"), 
			CONFIG_FLOAT("data.ingamefeedback.pickPolypeptides.movingPolypeptideColor.a")
		));
		
		auto polypeptides = screen->getSelectedPolypeptides();
		unsigned int size = polypeptides.getSize();
		for(unsigned i = 0; i < size; ++i)
		{
			float angle = 360.f / size;
			float distance = 15.f + size;
			Vec2f pos = position + Vec2f(cosf(toRadians(angle * i)) * distance, sinf(toRadians(angle * i)) * distance);
			gl::drawSolidCircle(position + Vec2f(cosf(toRadians(angle * i)) * distance, sinf(toRadians(angle * i)) * distance), 5.f);
		}
	}
	glPopAttrib();
}

bool GameScreenStateMovePolypeptides::touchClick(TouchWay touchWay)
{
	auto& containerMutex = GAME_SCR->getContainerMutex();

	auto selectedPolypeptidesSize = screen->getSelectedPolypeptides().getSize();

	auto cellsPicked = screen->getCellsPicked(touchWay.getCurrentPos());

	if (cellsPicked.size() > 0)
	{
		if (selectedPolypeptidesSize != 0)
		{
			containerMutex.lock();
			auto fromCell = screen->getSelectedPolypeptides().begin()->second->getOwner();
			containerMutex.unlock();

			auto toCell = cellsPicked[0];

			if (fromCell != nullptr && fromCell != toCell)
			{
				if (!(toCell->isFull()))
				{
					MovePolypeptideRequest * request = new MovePolypeptideRequest();
					request->fromCellId = fromCell->getId();
					request->toCellId = toCell->getId();
					request->amount = selectedPolypeptidesSize;
					request->endpoint = GAME_MGR->getServerEndpoint();
					NETWORK_MGR->registerMovePolypeptideRequest(request, 
						fromCell,
						toCell,
						selectedPolypeptidesSize);
					NETWORK_MGR->send(request);
					LOG_INFO("MovePolypeptideRequest sent");
				}
				return true;
			}
		}
	}

	containerMutex.lock();
	screen->getSelectedPolypeptides().clear();
	containerMutex.unlock();

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