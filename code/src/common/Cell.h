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

 float getHealthPoints() const { return healthPoints; }

 float getAngle() const { return ci::toRadians(rotation.z); }
 void setAngle(float angle) { rotation.z = ci::toDegrees(angle); }

 bool getIsComplete() const { return isComplete; }

 const Player * getOwner() const { return owner; }
 void setOwner( const Player * owner) { this->owner = owner; }

 std::map<unsigned int, Polypeptide *> & getPolypeptides()
 {
	 return polypeptides;
 }

 bool addPolypetide(Polypeptide * polypeptide)
 {
	 if (polypeptides.size() < polyMax)
	 {
		 return polypeptides.insert(make_pair(polypeptide->getId(), polypeptide)).second;
	 }
	 return false;
 }

 bool removePolypetide(Polypeptide * polypeptide)
 {
	 auto it = polypeptides.find(polypeptide->getId());
	 if (it != polypeptides.end())
	 {
		polypeptides.erase(it);
		return true;
	 }
	 return false;
 }

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

 virtual void setPosition(Vec3f position) { throw logic_error("Not implemented exception"); }
 virtual void setRotation(Vec3f rotation) { throw logic_error("Not implemented exception"); }
 virtual void setScale(Vec3f scale) { throw logic_error("Not implemented exception"); }
 virtual void setRadius(float radius) { throw logic_error("Not implemented exception"); }
};