#pragma once

#include <vector>

#include "GameObject.h"
#include "CellCollisionBehavior.h"

using namespace std;

class Cell : public GameObject
{
public:
	friend class Population;

	Cell() 
	{
		m_pCollisionBehavior = new CellCollisionBehavior();
	}

	Cell(unsigned int m_id, const float position[2])
	{
		Cell();
		setPosition(position);
	}

	Cell(const Cell& cpy)
	{
		m_uiId = cpy.m_uiId;
		setPosition(cpy.m_afPosition);
	}

	unsigned int getId() const { return m_uiId; }
	const float* const getPosition() const { return m_afPosition; }
	float getRadius() const { return m_fRadius; }
protected:
	unsigned int m_uiId;
	float m_afPosition[2];
	float m_fRadius;
	vector<Cell*> m_parents;
	vector<Cell*> m_children;

	void setPosition(const float position[2])
	{
		m_afPosition[0] = position[0];
		m_afPosition[1] = position[1];
	}
};