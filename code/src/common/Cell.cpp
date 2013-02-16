#include "Cell.h"

Cell::Cell() : maxHealthPoints(1.f), polyMax(CONFIG_INT2("data.polypetide.maxPerCell", 10))
{ }

Cell::Cell(Vec3f position, float angle, Player * owner) : owner(owner), maxHealthPoints(1.f), polyMax(CONFIG_INT2("data.polypetide.maxPerCell", 10))
{
 this->position = position;
 setAngle(angle);
}

Cell::Cell(Vec3f position, float radius, float angle, float healthPoints) :
 healthPoints(healthPoints),
 maxHealthPoints(healthPoints),
 polyMax(CONFIG_INT2("data.polypetide.maxPerCell", 10))
{
 this->position = position;
 this->radius = radius;
 this->rotation = Vec3f(0.0f, 0.0f, float(ci::toDegrees(angle)));
}

Cell::Cell(Vec3f position, float radius, float angle, float healthPoints, Player * owner) :
 healthPoints(healthPoints),
 polyMax(CONFIG_INT2("data.polypetide.maxPerCell", 10)),
 maxHealthPoints(healthPoints),
 owner(owner)
{
 this->position = position;
 this->radius = radius;
 this->rotation = Vec3f(0.0f, 0.0f, float(ci::toDegrees(angle)));
}

void Cell::decreaseHealthPointsBy(float damage)
{
 healthPoints -= damage;
 LOG_INFO(stringify(ostringstream() << "cell with id: " << id << " has still " << healthPoints << " healthpoints"));
}

void Cell::completeCell() { isComplete = true; }