#pragma once

#include <vector>

#include "GameObject.h"
#include "CellCollisionBehavior.h"

#define HEALTH_POINTS 100.f

class Player;

class Cell : public GameObject
{
public:
	Cell();

	void init(const Vec2f position, float angle);

	void decreaseHealthPointsBy(float damage);

	void completeCell();

	float getHealthPoints() const;
	float getAngle() const;
	float getRadius() const;
	bool isComplete() const;
	void setOwner( Player * owner);
	Player * getPlayer();
protected:
	float m_fHealthPoints;
	bool m_bIdSet;
	float m_fAngle;
	bool m_bIsInitialized;
	float m_fRadius;
	bool m_bIsComplete;
	Player * m_pOwner;

	Cell(float radius);
};
