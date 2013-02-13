#include "GameScreenStateCreateCell.h"
#include "GameScreenStateNeutral.h"
#include "../../managers/GameManager.h"
#include "../../../common/network/messages/game/ingame/cell/creation/CreateCellRequest.h"
#include "../../../common/network/messages/game/ingame/cell/creation/CreateCellSuccess.h"
#include "../../../common/network/messages/game/ingame/cell/creation/CreateCellFailure.h"
#include "../../actors/StandardCellClient.h"

GameScreenStateCreateCell::GameScreenStateCreateCell(GameScreen * screen, CellType::Type cellType):
	GameScreenState(screen),
	pickedCell(screen->pickedCell),
	cellType(cellType)
{
	if (pickedCell != nullptr)
	{
		switch(cellType)
		{
		case CellType::StandardCell:
		default:
			{
				cell = new StandardCellClient(0, Vec3f::zero(), 0, nullptr);
				break;
			}
		}

		cell->setOwner(GAME_MGR->getMyPlayer());
	}
	else
	{
		screen->switchToState(new GameScreenStateNeutral(screen));
	}
}

bool GameScreenStateCreateCell::mouseMove(MouseEvent event)
{
	auto mouseCoordinates3D = RenderManager::getInstance()->cam.screenToWorldPlane(event.getPos());
	auto pickedCellPosition = pickedCell->getPosition();
	auto pickedCellRadius = pickedCell->getRadius();
	auto direction = mouseCoordinates3D - pickedCellPosition;
	direction.normalize();
	angle = atan2(direction.y, direction.x);
		
	if (cell != nullptr)
	{
		auto cellPosition = pickedCellPosition + direction * (pickedCellRadius + cell->getRadius());
		cell->setPosition(cellPosition);
		cell->setAngle(angle);
	}

	arrowStart = pickedCellPosition + direction * (pickedCellRadius + 5);
	arrowEnd = arrowStart + direction * 50;

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

	GAME_SCR.addCellPreview(cell);

	NETWORK_MGR->send(createCellRequest);
	LOG_INFO("CreateCellRequest sent");

	screen->switchToState(new GameScreenStateNeutral(screen));

	return false;	
}