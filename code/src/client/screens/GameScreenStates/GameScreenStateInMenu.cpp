#include "GameScreenStateInMenu.h"
#include "GameScreenStateNeutral.h"
#include "../../rendering/RenderManager.h"
#include "../../actors/StemCellClient.h"
#include "GameScreenStateSelectPolypeptides.h"
#include "../../../common/PolypeptideCapacityContainer.h"
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
			auto createCellButtonAngle = CONFIG_FLOAT("data.menu.ingame.create." + name + ".angle") / 180.0 * M_PI;
			auto cellEdgePoint3D = pickedCell->getPosition() + Vec3f((float) cos(createCellButtonAngle) * pickedCell->getRadius(), (float) sin(createCellButtonAngle) * pickedCell->getRadius(), 0.f);
			auto cellEdgePoint2D = RenderManager::getInstance()->cam.worldToScreen(cellEdgePoint3D, (float) getWindowWidth(), (float) getWindowHeight());
			auto createCellButtonDistance = CONFIG_FLOAT("data.menu.ingame.create." + name + ".distance") + createCellButtonRadius;
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

bool GameScreenStateInMenu::touchBegan(const TouchWay & touchWay)
{
	return GameScreenState::touchBegan(touchWay);
}

void GameScreenStateInMenu::touchMoved(const TouchWay & touchWay)
{
	bool isMouseOverMenuItem = false;
	auto cellMenuButtons = screen->cellMenuButtons;

	for (auto it = cellMenuButtons.begin(); it != cellMenuButtons.end(); ++it)
	{
		if (isMouseOverMenuItem |= it->second->isMouseOverItem(touchWay.getCurrentPos()))
			return;
	}

	GameScreenState::touchMoved(touchWay);

	for (auto it = cellMenuButtons.begin(); it != cellMenuButtons.end(); ++it)
	{
		auto button = it->second;
		button->setPosition(button->getPosition() + touchWay.getLastDeltaVector());
	}
}

bool GameScreenStateInMenu::touchClick(TouchWay touchWay)
{
	
	if (touchWay.getTrigger() == TouchWay::LEFT)
	{
		auto cellsPicked = screen->getCellsPicked(touchWay.getCurrentPos());

		if (cellsPicked.size() > 0 && cellsPicked[0] == pickedCell)
		{
			screen->switchToState(new GameScreenStateNeutral(screen));
			return false;
		}
	}

	return GameScreenState::touchClick(touchWay);
}

void GameScreenStateInMenu::update(float frametime)
{
	screen->cellMenu->setVisible(true);

	bool ownCellFull = pickedCell->getPolypeptides().size() >= pickedCell->getMaxNumOfPolys();
	bool globalCapacityFull = POLYCAPACITY->isFull();

	if (!isStemcell || ownCellFull || globalCapacityFull)
	{
		screen->cellMenuButtons["polypeptid"]->setVisible(false);
	}
}