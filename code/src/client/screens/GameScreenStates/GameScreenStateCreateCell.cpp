#include "GameScreenStateCreateCell.h"
#include "GameScreenStateNeutral.h"
#include "../../managers/GameManager.h"
#include "../../../common/network/messages/game/ingame/cell/creation/CreateCellRequest.h"
#include "../../../common/network/messages/game/ingame/cell/creation/CreateCellSuccess.h"
#include "../../../common/network/messages/game/ingame/cell/creation/CreateCellFailure.h"
#include "../../actors/StandardCellClient.h"
#include "../../actors/BoneCellClient.h"

GameScreenStateCreateCell::GameScreenStateCreateCell(GameScreen * screen, CellClient * pickedCell, CellClient * cell):
	GameScreenState(screen),
	cellType(cell->getType()),
	cell(cell)
{
	if (pickedCell != nullptr)
	{
		screen->pickedCell = pickedCell;
		cell->setOwner(GAME_MGR->getMyPlayer());
	}
	else
	{
		screen->switchToState(new GameScreenStateNeutral(screen));
		cell->setOpacity(CONFIG_FLOAT("data.ingamefeedback.building.previewOpacity"));
	}
}

GameScreenStateCreateCell::GameScreenStateCreateCell(GameScreen * screen, CellType::Type cellType):
	GameScreenState(screen),
	cellType(cellType)
{
	if (screen->pickedCell != nullptr)
	{
		switch(cellType)
		{
		case CellType::StandardCell:
		default:
			{
				cell = new StandardCellClient(0, Vec3f::zero(), 0, nullptr);
				break;
			}
		case CellType::BoneCell:
			{
				cell = new BoneCellClient(0, Vec3f::zero(), 0, nullptr);
				break;
			}
		}

		cell->setOpacity(CONFIG_FLOAT("data.ingamefeedback.building.previewOpacity"));
		cell->setHue(GAME_MGR->getMyHue());
		cell->setOwner(GAME_MGR->getMyPlayer());
		cell->show();
	}
	else
	{
		screen->switchToState(new GameScreenStateNeutral(screen));
	}
}

bool GameScreenStateCreateCell::mouseMove(MouseEvent event)
{
	auto pickedCell = screen->pickedCell;

	if (pickedCell != nullptr)
	{
		auto mouseCoordinates3D = RenderManager::getInstance()->cam.screenToWorldPlane(event.getPos());
		auto pickedCellPosition = pickedCell->getPosition();
		auto pickedCellRadius = pickedCell->getRadius();
		auto direction = mouseCoordinates3D - pickedCellPosition;
		direction.normalize();
		angle = atan2(direction.y, direction.x);
		
		if (cell != nullptr)
		{
			auto cellPosition = pickedCellPosition + direction * (pickedCellRadius * 1.01f + cell->getRadius());
			cell->setPosition(cellPosition);
			cell->setAngle(angle);

			if (!screen->canCellBePlaced(cell))
			{
				cell->setHue(CONFIG_FLOAT("data.ingamefeedback.building.collisionHue"));
			}
			else
			{
				cell->setHue(CONFIG_FLOAT("data.ingamefeedback.building.noCollisionHue"));
			}
		}

		arrowStart = pickedCellPosition + direction * (pickedCellRadius + 5);
		arrowEnd = arrowStart + direction * 50;
	}
	else
	{
		screen->switchToState(new GameScreenStateNeutral(screen));
	}

	return false;
}

void GameScreenStateCreateCell::draw3D()
{
	if (cell != nullptr)
	{
		cell->draw();
	}
}

bool GameScreenStateCreateCell::touchBegan(const TouchWay & touchWay)
{
	if (touchWay.getTrigger() == TouchWay::LEFT)
	{
		auto pickedCell = screen->pickedCell;

		if (pickedCell != nullptr)
		{	
			auto createCellRequest = new CreateCellRequest();
			createCellRequest->endpoint = GAME_MGR->getServerEndpoint();
			createCellRequest->playerId = pickedCell->getOwner()->getId();
			createCellRequest->cellId = pickedCell->getId();
			createCellRequest->angle = angle;
			createCellRequest->type = cellType;

			NETWORK_MGR->registerCreateCellRequest(
				createCellRequest,
				cell,
				pickedCell
			);

			GAME_SCR->addCellPreview(cell);

			LOG_INFO("CREATE CELL REQUEST:");
			LOG_INFO("PLAYER ID:");
			LOG_INFO(createCellRequest->playerId);
			LOG_INFO("CELL ID:");
			LOG_INFO(createCellRequest->cellId);
			LOG_INFO("ANGLE");
			LOG_INFO(createCellRequest->angle);
			LOG_INFO("TYPE");
			LOG_INFO(createCellRequest->type.getType());

			NETWORK_MGR->send(createCellRequest);
			LOG_INFO("CreateCellRequest sent");
		}
	}

	screen->switchToState(new GameScreenStateNeutral(screen));

	return false;
}

void GameScreenStateCreateCell::touchMoved(const TouchWay & touchWay)
{
}