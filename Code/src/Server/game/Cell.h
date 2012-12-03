#pragma once

#include <vector>

#include "GameObject.h"
#include "CellCollisionBehavior.h"

using namespace std;

class Cell : public GameObject
{
public:
	Cell() :
	  m_bIdSet(false),
	  m_bPositionSet(false)
	{
		m_pCollisionBehavior = new CellCollisionBehavior();
	}

	Cell(const Cell& cpy)
	{
		m_uiId = cpy.m_uiId;
		setPosition(cpy.m_afPosition);
	}

	void setId(unsigned int id)
	{
		if(!m_bIdSet)
		{
			m_bIdSet = true;
			m_uiId = id;
		}
	}

	void setPosition(const float position[2])
	{
		if(!m_bPositionSet)
		{
			m_bPositionSet = true;
			m_afPosition[0] = position[0];
			m_afPosition[1] = position[1];
		}
	}

	void completeCell() { m_bIsComplete = true; }

	unsigned int getId() const { return m_uiId; }
	const float* const getPosition() const { return m_afPosition; }
	float getRadius() const { return m_fRadius; }
	bool isComplete() const { return m_bIsComplete; }
protected:
	unsigned int m_uiId;
	bool m_bIdSet;
	float m_afPosition[2];
	bool m_bPositionSet;
	float m_fRadius;
	bool m_bIsComplete;
	vector<Cell*> m_parents;
	vector<Cell*> m_children;

	Cell(float radius) : m_fRadius(radius) 
	{
		Cell();
	}
};