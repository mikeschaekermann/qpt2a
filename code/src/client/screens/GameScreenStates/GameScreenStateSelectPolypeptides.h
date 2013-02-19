#pragma once
#include "GameScreenState.h"

class GameScreenStateSelectPolypeptides :
	public GameScreenState
{
public:
	GameScreenStateSelectPolypeptides(GameScreen * screen, CellClient * pickedCell);

	virtual bool touchClick(TouchWay touchWay);
	virtual void touchMoved(const TouchWay & touchWay);
	virtual void touchEnded(const TouchWay & touchWay);
	virtual void draw2D();

private:
	bool hasDragged;
	CellClient * pickedCell;
	Vec2f center;
	float	maxPickRadius,
			pickRadius;
	unsigned int polypeptideCount;
	float onePolypeptideDistance;
	unsigned int numberOfPolypeptidesSelected;
};
