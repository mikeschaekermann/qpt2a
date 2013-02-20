#pragma once

#include <vector>
#include <map>
#include <stdexcept>

#include "GameObject.h"
#include "Polypeptide.h"


#include "../common/ConfigurationDataHandler.h"

class Player;

class Cell :
 virtual public GameObject
{
public:
 void decreaseHealthPointsBy(float damage);

 void completeCell();

 float getHealthPoints() const;

 float getAngle() const;
 void setAngle(float angle);

 bool getIsComplete() const;

 const Player * getOwner() const;
 void setOwner( const Player * owner);

 std::map<unsigned int, Polypeptide *> & getPolypeptides();
 bool addPolypeptide(Polypeptide * polypeptide);
 bool removePolypeptide(Polypeptide * polypeptide);
 unsigned int getMaxNumOfPolys() const;
 bool isFull();

protected:
 float healthPoints;
 const float maxHealthPoints;
 bool isComplete;
 const Player * owner;
 unsigned int polyMax;
 std::map<unsigned int, Polypeptide *> polypeptides;

 Cell(void);
 Cell(Vec3f position, float angle, Player * owner);
 Cell(Vec3f position, float radius, float angle, float healthPoints);
 Cell(Vec3f position, float radius, float angle, float healthPoints, Player * owner);

 virtual void setPosition(Vec3f position);
 virtual void setRotation(Vec3f rotation);
 virtual void setScale(Vec3f scale);
 virtual void setRadius(float radius);
};