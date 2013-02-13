#include "GameScreenStateInMenu.h"
#include "GameScreenStateNeutral.h"
#include "../../rendering/RenderManager.h"

GameScreenStateInMenu::GameScreenStateInMenu(GameScreen* screen, CellClient * pickedCell):
	GameScreenState(screen),
	pickedCell(pickedCell)
{
	if (pickedCell != nullptr)
	{
		screen->pickedCell = pickedCell;
		auto menuPosition3D = pickedCell->getPosition() + Vec3f(pickedCell->getRadius() + 5, 0, 0);
		auto menuPosition2D = RenderManager::getInstance()->cam.worldToScreen(menuPosition3D, getWindowWidth(), getWindowHeight());
		screen->cellMenu->setPosition(menuPosition2D);
		screen->cellMenu->setVisible(true);
	}
	else
	{
		screen->switchToState(new GameScreenStateNeutral(screen));
	}
}

GameScreenStateInMenu::~GameScreenStateInMenu(void)
{
	screen->cellMenu->setVisible(false);
}

bool GameScreenStateInMenu::touchClick(TouchWay touchWay)
{
	auto cellsPicked = screen->getCellsPicked(touchWay.getCurrentPos());

	if (cellsPicked.size() == 0)
	{
		screen->switchToState(new GameScreenStateNeutral(screen));
		return false;
	}
	else if (cellsPicked[0] != pickedCell)
	{
		screen->switchToState(new GameScreenStateInMenu(screen, cellsPicked[0]));
	}

	return false;
}