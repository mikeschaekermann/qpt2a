#include "Cell.h"

Cell::Cell(unsigned id) :
	GameObject(id),
	m_fHealthPoints(HEALTH_POINTS),
	m_bIsInitialized(false)
{
	setCollisionBehavior(new CellCollisionBehavior());
}

// warum eine init methode?
void Cell::init(const Vec2f position, float angle)
{
	if (!m_bIsInitialized)
	{
		m_bIsInitialized = true;
		setPosition(Vec3f(position));
		m_fAngle = angle;
	}
}

void Cell::decreaseHealthPointsBy(float damage)
{
	m_fHealthPoints -= damage;
}

void Cell::completeCell() { m_bIsComplete = true; }

float Cell::getHealthPoints() const
{
	return m_fHealthPoints;
}

float Cell::getAngle() const
{
	return m_fAngle;
}

float Cell::getRadius() const
{
	return m_fRadius;
}

bool Cell::isComplete() const
{
	return m_bIsComplete;
}

void Cell::setOwner( Player * owner)
{
	if (owner != nullptr) { m_pOwner = owner; }
}

Player * Cell::getPlayer()
{
	return m_pOwner;
}