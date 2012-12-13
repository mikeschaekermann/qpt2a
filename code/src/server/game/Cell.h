#pragma once

#include <vector>

#include "GameObject.h"
#include "../../common/CellCollisionBehavior.h"

#define HEALTH_POINTS 100.f

using namespace std;

class Cell : public GameObject
{
public:
	Cell() :
	  m_fHealthPoints(HEALTH_POINTS),
	  m_bIdSet(false),
	  m_bIsInitialized(false)
	{
		m_pCollisionBehavior = new CellCollisionBehavior();
	}

	Cell(const Cell& cpy)
	{
		m_uiId = cpy.m_uiId;
		m_afPosition[0] = cpy.m_afPosition[0];
		m_afPosition[1] = cpy.m_afPosition[1];
	}

	void init(const float position[2], float angle)
	{
		if (!m_bIsInitialized)
		{
			m_bIsInitialized = true;
			m_afPosition[0] = position[0];
			m_afPosition[1] = position[1];
			m_fAngle = angle;
		}
	}

	void setId(unsigned int id)
	{
		if(!m_bIdSet)
		{
			m_bIdSet = true;
			m_uiId = id;
		}
	}

	void decreaseHealthPointsBy(float damage)
	{
		m_fHealthPoints -= damage;
	}

	void completeCell() { m_bIsComplete = true; }

	unsigned int getId() const { return m_uiId; }
	float getHealthPoints() const { return m_fHealthPoints; }
	const float* const getPosition() const { return m_afPosition; }
	float getAngle() const { return m_fAngle; }
	float getRadius() const { return m_fRadius; }
	bool isComplete() const { return m_bIsComplete; }
protected:
	unsigned int m_uiId;
	float m_fHealthPoints;
	bool m_bIdSet;
	float m_afPosition[2];
	float m_fAngle;
	bool m_bIsInitialized;
	float m_fRadius;
	bool m_bIsComplete;
	vector<Cell*> m_parents;
	vector<Cell*> m_children;

	Cell(float radius) : m_fRadius(radius) 
	{
		Cell();
	}
};