#pragma once
#include "../../managers/GameManager.h"
#include "GameScreenState.h"
#include "../../../common/network/messages/enum/CellType.h"
#include "../../../common/network/messages/game/ingame/cell/creation/CreateCellRequest.h"
#include "../../../common/network/messages/game/ingame/cell/creation/CreateCellSuccess.h"
#include "../../../common/network/messages/game/ingame/cell/creation/CreateCellFailure.h"
#include "../../actors/StandardCellClient.h"

class GameScreenStateCreateCell :
	public GameScreenState
{
public:
	GameScreenStateCreateCell(GameScreen * screen, CellType::Type cellType):
		GameScreenState(screen),
		cellType(cellType)
	{
		screen->cellMenu->setVisible(false);

		switch(cellType)
		{
		case CellType::StandardCell:
		default:
			{
				previewCell = new StandardCellClient(0, Vec3f::zero(), 0, nullptr);
				break;
			}
		}
	}

	virtual bool mouseMove(MouseEvent event)
	{
		auto mouseCoordinates3D = screen->cam.screenToWorldPlane(event.getPos());
		auto pickedCell = screen->pickedCell;
		auto pickedCellPosition = pickedCell->getPosition();
		auto pickedCellRadius = pickedCell->getRadius();
		auto direction = mouseCoordinates3D - pickedCellPosition;
		direction.normalize();
		angle = atan2(direction.y, direction.x);
		
		if (previewCell != nullptr)
		{
			auto previewCellPosition = pickedCellPosition + direction * (pickedCellRadius + previewCell->getRadius());
			previewCell->setPosition(previewCellPosition);
			previewCell->setAngle(angle);
		}

		arrowStart = pickedCellPosition + direction * (pickedCellRadius + 5);
		arrowEnd = arrowStart + direction * 50;

		return false;
	}

	virtual void draw3D()
	{
		if (previewCell != nullptr)
		{
			gl::color(1, 1, 1, 0.5);
			previewCell->draw();
		}

		gl::color(0, 0, 1);
		gl::drawVector(arrowEnd, arrowStart);
	}

	virtual bool touchBegan(const TouchWay & touchWay)
	{
		auto createCellRequest = new CreateCellRequest();
		createCellRequest->endpoint = GAME_MGR->getServerEndpoint();
		createCellRequest->playerId = screen->pickedCell->getOwner()->getId();
		createCellRequest->cellId = screen->pickedCell->getId();
		createCellRequest->angle = angle;
		createCellRequest->type = cellType;

		NETWORK_MGR->registerCreateCellCallbacks(
			createCellRequest,
			[](CreateCellSuccess *){
				LOG_INFO("CreateCellSuccess received!");
			},
			[](CreateCellFailure *){
				LOG_INFO("CreateCellFailure received!");
			}
		);

		NETWORK_MGR->send(createCellRequest);

		return false;	
	}

protected:
	float angle;
	CellClient * previewCell;
	Vec3f arrowStart, arrowEnd;
	CellType::Type cellType;
};

