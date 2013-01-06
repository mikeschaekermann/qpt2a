#include "Cell.h"

Cell::Cell() { }

Cell::Cell(Vec3f position, float radius, float angle, float healthPoints) :
	healthPoints(healthPoints)
{
	this->position = position;
	this->radius = radius;
	this->rotation = Vec3f(0.0f, 0.0f, float(angle / 180.0f * M_PI));
}

void Cell::decreaseHealthPointsBy(float damage)
{
	healthPoints -= damage;
}

void Cell::completeCell() { isComplete = true; }

