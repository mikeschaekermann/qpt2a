#include "GameScreenStateInMenu.h"
#include "GameScreenStateNeutral.h"
#include "../../rendering/RenderManager.h"
#include "../../actors/StemCellClient.h"

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
			auto cellEdgePoint3D = pickedCell->getPosition() + Vec3f((float) cos(createCellButtonAngle) * pickedCell->getRadius(), (float) sin(createCellButtonAngle) * pickedCell->getRadius(), 0.f);
			auto cellEdgePoint2D = RenderManager::getInstance()->cam.worldToScreen(cellEdgePoint3D, (float) getWindowWidth(), (float) getWindowHeight());
			auto createCellButtonDistance = CONFIG_FLOAT2("data.menu.ingame.create." + name + ".distance", 30) + createCellButtonRadius;
			auto createCellButtonOffsetFromEdge2D = Vec2f((float) (cos(createCellButtonAngle) * createCellButtonDistance), (float) (-sin(createCellButtonAngle) * createCellButtonDistance));
			auto createCellButtonCenter = cellEdgePoint2D + createCellButtonOffsetFromEdge2D;
			auto createCellButtonOffset = createCellButtonSize / (-2);
			auto createCellButtonPosition = createCellButtonCenter + createCellButtonOffset;
		
			button->setPosition(createCellButtonPosition);
		}

		screen->cellMenu->setVisible(true);
		isStemcell = dynamic_cast<StemCellClient *>(pickedCell) != nullptr;
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
	if (touchWay.getTrigger() == TouchWay::LEFT)
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
	}
	else
	{
		screen->switchToState(new GameScreenStateNeutral(screen));
	}

	return false;
}

bool GameScreenStateInMenu::mouseMove(MouseEvent event)
{
	screen->cellMenu->setVisible(true);
	if (!isStemcell)
	{
		screen->cellMenuButtons["polypeptid"]->setVisible(false);
	}

	return false;
}