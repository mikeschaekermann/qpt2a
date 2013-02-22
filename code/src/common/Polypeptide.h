#pragma once

#include <vector>
#include <stdexcept>

#include "GameObject.h"

#include "boost/thread/mutex.hpp"

class Cell;

class Polypeptide :
 virtual public GameObject
{
public:
	enum State
	{
		TRAVEL,
		IDLE,
		CELLFIGHT,
		POLYPEPTIDEFIGHT
	};

	void setState(State state) { this->state = state; }
	State getState() const { return state; }

	float getAngle() const { return ci::toRadians(rotation.z); }
	void setAngle(float angle) { rotation.z = ci::toDegrees(angle); }

	const Cell * getOwner() const { return owner; }
	void setOwner( Cell * owner) { this->owner = owner; }

	static boost::mutex& getMutex() { return polyMutex; }

protected:
 State state;
 Cell * owner;
 static boost::mutex polyMutex;

 Polypeptide(void);
 Polypeptide(Vec3f position, float angle, Cell * owner);
 Polypeptide(Vec3f position, float radius, float angle);
 Polypeptide(Vec3f position, float radius, float angle, Cell * owner);

 virtual void setPosition(Vec3f position) { throw logic_error("Not implemented exception"); }
 virtual void setRotation(Vec3f rotation) { throw logic_error("Not implemented exception"); }
 virtual void setScale(Vec3f scale) { throw logic_error("Not implemented exception"); }
 virtual void setRadius(float radius) { throw logic_error("Not implemented exception"); }
};