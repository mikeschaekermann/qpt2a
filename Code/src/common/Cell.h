#pragma once

#include <vector>

#include "GameObject.h"
#include "CellCollisionBehavior.h"
#include "Player.h"

#define HEALTH_POINTS 100.f

class Cell : public GameObject
{
public:
	Cell() :
		GameObject(),
		m_fHealthPoints(HEALTH_POINTS),
		m_bIsInitialized(false)
	{
		setCollisionBehavior(new CellCollisionBehavior());
	}

	void init(const Vec2f position, float angle)
	{
		if (!m_bIsInitialized)
		{
			m_bIsInitialized = true;
			setPosition(Vec3f(position));
			m_fAngle = angle;
		}
	}

	void decreaseHealthPointsBy(float damage)
	{
		m_fHealthPoints -= damage;
	}

	void completeCell() { m_bIsComplete = true; }

	float getHealthPoints() const { return m_fHealthPoints; }
	float getAngle() const { return m_fAngle; }
	float getRadius() const { return m_fRadius; }
	bool isComplete() const { return m_bIsComplete; }
	void setOwner( Player * owner) { if (owner != nullptr) { m_pOwner = owner; } }
	Player * getPlayer() { return m_pOwner; }
protected:
	float m_fHealthPoints;
	bool m_bIdSet;
	float m_fAngle;
	bool m_bIsInitialized;
	float m_fRadius;
	bool m_bIsComplete;
	Player * m_pOwner;

	Cell(float radius) : m_fRadius(radius) 
	{
		Cell();
	}
};