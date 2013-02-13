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

		auto cellMenuButtons = screen->cellMenuButtons;

		for (auto it = cellMenuButtons.begin(); it != cellMenuButtons.end(); ++it)
		{
			auto name = it->first;
			auto button = it->second;

			auto createCellButtonSize = ASSET_MGR->getGuiTexture(string("ingame-button-" + name + "-normal")).getSize();
			auto createCellButtonRadius = 0.25 * (createCellButtonSize.x + createCellButtonSize.y);
			auto createCellButtonAngle = CONFIG_FLOAT2("data.menu.ingame.create." + name + ".angle", 45) / 180.0 * M_PI;
			auto cellEdgePoint3D = pickedCell->getPosition() + Vec3f(cos(createCellButtonAngle) * pickedCell->getRadius(), sin(createCellButtonAngle) * pickedCell->getRadius(), 0);
			auto cellEdgePoint2D = RenderManager::getInstance()->cam.worldToScreen(cellEdgePoint3D, getWindowWidth(), getWindowHeight());
			auto createCellButtonDistance = CONFIG_FLOAT2("data.menu.ingame.create." + name + ".distance", 30) + createCellButtonRadius;
			auto createCellButtonOffsetFromEdge2D = Vec2f(cos(createCellButtonAngle) * createCellButtonDistance, -sin(createCellButtonAngle) * createCellButtonDistance);
			auto createCellButtonCenter = cellEdgePoint2D + createCellButtonOffsetFromEdge2D;
			auto createCellButtonOffset = - 0.5 * createCellButtonSize;
			auto createCellButtonPosition = createCellButtonCenter + createCellButtonOffset;
		
			button->setPosition(createCellButtonPosition);
		}

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