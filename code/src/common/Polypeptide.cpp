#include "Polypeptide.h"

Polypeptide::Polypeptide() : state(IDLE)
{ }

Polypeptide::Polypeptide(Vec3f position, float angle, Cell * owner) : owner(owner), state(IDLE)
{
 this->position = position;
 setAngle(angle);
}

Polypeptide::Polypeptide(Vec3f position, float radius, float angle) : state(IDLE)
{
 this->position = position;
 this->radius = radius;
 this->rotation = Vec3f(0.0f, 0.0f, float(ci::toDegrees(angle)));
}

Polypeptide::Polypeptide(Vec3f position, float radius, float angle, Cell * owner) :
 owner(owner),
 state(IDLE)
{
 this->position = position;
 this->radius = radius;
 this->rotation = Vec3f(0.0f, 0.0f, float(ci::toDegrees(angle)));
}