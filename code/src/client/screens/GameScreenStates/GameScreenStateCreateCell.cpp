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
	}
	else
	{
		screen->switchToState(new GameScreenStateNeutral(screen));
	}
}

bool GameScreenStateCreateCell::mouseMove(MouseEvent event)
{
	auto mouseCoordinates3D = screen->cam.screenToWorldPlane(event.getPos());
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
		gl::color(1, 1, 1, 0.5);
		cell->draw();
	}

	gl::color(0, 0, 1);
	gl::drawVector(arrowEnd, arrowStart);
}

bool GameScreenStateCreateCell::touchBegan(const TouchWay & touchWay)
{
	auto createCellRequest = new CreateCellRequest();
	createCellRequest->endpoint = GAME_MGR->getServerEndpoint();
	createCellRequest->playerId = pickedCell->getOwner()->getId();
	createCellRequest->cellId = pickedCell->getId();
	createCellRequest->angle = angle;
	createCellRequest->type = cellType;

	NETWORK_MGR->registerCreateCellCallbacks(
		createCellRequest,
		[this](CreateCellSuccess * response){
			LOG_INFO("CreateCellSuccess received");

			cell->setPosition(response->position);
			cell->setAngle(response->angle);
			cell->setId(response->cellId);
			cell->addParent(pickedCell);
			pickedCell->addChild(cell);

			GAME_SCR.addIncompleteCell(cell);

			screen->switchToState(new GameScreenStateNeutral(screen));
		},
		[this](CreateCellFailure * response){
			LOG_INFO("CreateCellFailure received");

			delete cell;
		}
	);

	NETWORK_MGR->send(createCellRequest);
	LOG_INFO("CreateCellRequest sent");

	return false;	
}