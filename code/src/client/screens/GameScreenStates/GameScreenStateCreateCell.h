#pragma once
#include "GameScreenState.h"
#include "../../actors/StemCellClient.h"

class GameScreenStateCreateCell :
	public GameScreenState
{
public:
	GameScreenStateCreateCell(GameScreen * screen):
		GameScreenState(screen),
		previewCell(nullptr)
	{
		screen->cellMenu->setVisible(false);
	}

	virtual bool mouseMove(MouseEvent event)
	{
		auto mouseCoordinates3D = screen->cam.screenToWorldPlane(event.getPos());
		auto pickedCell = screen->pickedCell;
		auto pickedCellPosition = pickedCell->getPosition();
		auto pickedCellRadius = pickedCell->getRadius();
		auto direction = mouseCoordinates3D - pickedCellPosition;
		direction.normalize();
		
		if (previewCell != nullptr)
		{
			auto previewCellPosition = pickedCellPosition + direction * (pickedCellRadius + previewCell->getRadius());
			previewCell->setPosition(previewCellPosition);
			previewCell->setAngle(atan2(direction.y, direction.x));
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

protected:
	CellClient * previewCell;
	Vec3f arrowStart, arrowEnd;
};

