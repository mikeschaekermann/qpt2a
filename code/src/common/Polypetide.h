#pragma once

#include <vector>
#include <stdexcept>

#include "GameObject.h"

class Player;

class Polypetide :
 virtual public GameObject
{
public:
 void decreaseHealthPointsBy(float damage);

 float getHealthPoints() const { return healthPoints; }

 float getAngle() const { return ci::toRadians(rotation.z); }
 void setAngle(float angle) { rotation.z = ci::toDegrees(angle); }

 const Player * getOwner() const { return owner; }
 void setOwner( const Player * owner) { this->owner = owner; }
protected:
 float healthPoints;
 const float maxHealthPoints;
 const Player * owner;

 Polypetide(void);
 Polypetide(Vec3f position, float angle, Player * owner);
 Polypetide(Vec3f position, float radius, float angle, float healthPoints);
 Polypetide(Vec3f position, float radius, float angle, float healthPoints, Player * owner);

 virtual void setPosition(Vec3f position) { throw logic_error("Not implemented exception"); }
 virtual void setRotation(Vec3f rotation) { throw logic_error("Not implemented exception"); }
 virtual void setScale(Vec3f scale) { throw logic_error("Not implemented exception"); }
 virtual void setRadius(float radius) { throw logic_error("Not implemented exception"); }
};