#include "Cell.h"

#include "ConfigurationDataHandler.h"


Cell::Cell(Vec3f position, float radius, float angle, float healthPoints) :
	healthPoints(healthPoints)
{
	this->position = position;
	this->radius = radius;
	this->angle = angle;
}

void Cell::decreaseHealthPointsBy(float damage)
{
	healthPoints -= damage;
}

void Cell::completeCell() { isComplete = true; }