#include "Polypetide.h"

Polypetide::Polypetide() : maxHealthPoints(1.f)
{ }

Polypetide::Polypetide(Vec3f position, float angle, Player * owner) : owner(owner), maxHealthPoints(1.f)
{
 this->position = position;
 setAngle(angle);
}

Polypetide::Polypetide(Vec3f position, float radius, float angle, float healthPoints) :
 healthPoints(healthPoints),
 maxHealthPoints(healthPoints)
{
 this->position = position;
 this->radius = radius;
 this->rotation = Vec3f(0.0f, 0.0f, float(ci::toDegrees(angle)));
}

Polypetide::Polypetide(Vec3f position, float radius, float angle, float healthPoints, Player * owner) :
 healthPoints(healthPoints),
	 
 maxHealthPoints(healthPoints),
 owner(owner)
{
 this->position = position;
 this->radius = radius;
 this->rotation = Vec3f(0.0f, 0.0f, float(ci::toDegrees(angle)));
}

void Polypetide::decreaseHealthPointsBy(float damage)
{
 healthPoints -= damage;
 LOG_INFO(stringify(ostringstream() << "Polypetide with id: " << id << " has still " << healthPoints << " healthpoints"));
}