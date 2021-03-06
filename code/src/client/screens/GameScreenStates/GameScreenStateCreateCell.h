#pragma once

#include "GameScreenState.h"
#include "../../../common/network/messages/enum/CellType.h"

class CellClient;

class GameScreenStateCreateCell :
	public GameScreenState
{
public:
	GameScreenStateCreateCell(GameScreen * screen, CellType::Type cellType);
	GameScreenStateCreateCell(GameScreen * screen, CellClient * pickedCell, CellClient * cell);

	virtual bool mouseMove(MouseEvent event);
	virtual void draw3D();
	virtual bool touchBegan(const TouchWay & touchWay);
	virtual void touchMoved(const TouchWay & touchWay);

protected:
	CellClient * pickedCell;
	float angle;
	CellClient * cell;
	Vec3f arrowStart, arrowEnd;
	CellType::Type cellType;
};
