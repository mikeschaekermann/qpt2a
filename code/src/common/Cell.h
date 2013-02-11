#pragma once

#include <vector>
#include <stdexcept>

#include "GameObject.h"

class Player;

class Cell :
 virtual public GameObject
{
public:
 void decreaseHealthPointsBy(float damage);

 void completeCell();

 float getHealthPoints() const { return healthPoints; }

 float getAngle() const { return ci::toDegrees(rotation.z); }
 void setAngle(float angle) { rotation.z = ci::toRadians(angle); }

 bool getIsComplete() const { return isComplete; }

 const Player * getOwner() const { return owner; }
 void setOwner( const Player * owner) { this->owner = owner; }
protected:
 float healthPoints;
 bool isComplete;
 const Player * owner;

 Cell(void);
 Cell(Vec3f position, float angle, Player * owner);
 Cell(Vec3f position, float radius, float angle, float healthPoints);
 Cell(Vec3f position, float radius, float angle, float healthPoints, Player * owner);

 virtual void setPosition(Vec3f position) { throw logic_error("Not implemented exception"); }
 virtual void setRotation(Vec3f rotation) { throw logic_error("Not implemented exception"); }
 virtual void setScale(Vec3f scale) { throw logic_error("Not implemented exception"); }
 virtual void setRadius(float radius) { throw logic_error("Not implemented exception"); }
};