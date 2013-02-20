#include "Cell.h"

Cell::Cell() : maxHealthPoints(1.f), polyMax(0)
{ }

Cell::Cell(Vec3f position, float angle, Player * owner) :
	owner(owner),
	maxHealthPoints(1.f),
	polyMax(0)
{
 this->position = position;
 setAngle(angle);
}

Cell::Cell(Vec3f position, float radius, float angle, float healthPoints) :
 healthPoints(healthPoints),
 maxHealthPoints(healthPoints),
 polyMax(0)
{
 this->position = position;
 this->radius = radius;
 this->rotation = Vec3f(0.0f, 0.0f, float(ci::toDegrees(angle)));
}

Cell::Cell(Vec3f position, float radius, float angle, float healthPoints, Player * owner) :
 healthPoints(healthPoints),
 polyMax(0),
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

void Cell::setPosition(Vec3f position) { throw logic_error("Not implemented exception"); }
void Cell::setRotation(Vec3f rotation) { throw logic_error("Not implemented exception"); }
void Cell::setScale(Vec3f scale) { throw logic_error("Not implemented exception"); }
void Cell::setRadius(float radius) { throw logic_error("Not implemented exception"); }

float Cell::getHealthPoints() const { return healthPoints; }

float Cell::getAngle() const { return ci::toRadians(rotation.z); }
void Cell::setAngle(float angle) { rotation.z = ci::toDegrees(angle); }

bool Cell::getIsComplete() const { return isComplete; }

const Player * Cell::getOwner() const { return owner; }
void Cell::setOwner( const Player * owner) { this->owner = owner; }

std::map<unsigned int, Polypeptide *> & Cell::getPolypeptides()
{
	return polypeptides;
}

bool Cell::addPolypeptide(Polypeptide * polypeptide)
{
	if (polypeptides.size() < polyMax)
	{
		polypeptide->setOwner(this);
		return polypeptides.insert(make_pair(polypeptide->getId(), polypeptide)).second;
	}
	return false;
}

bool Cell::removePolypeptide(Polypeptide * polypeptide)
{
	auto it = polypeptides.find(polypeptide->getId());
	if (it != polypeptides.end())
	{
	polypeptides.erase(it);
	return true;
	}
	return false;
}

unsigned int Cell::getMaxNumOfPolys() const
{
	return polyMax;
}