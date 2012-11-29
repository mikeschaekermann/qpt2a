#pragma once
#include "GameObject.h"
class Cell :
	public GameObject
{
public:
	Cell(unsigned id, Player* player, Vec3f position = Vec3f(0, 0, 0), Vec3f rotation = Vec3f(0, 0, 0), Vec3f scale = Vec3f(1, 1, 1));
	virtual ~Cell(void);

	float getHealthPoints() const { return m_fHealthPoints; };

private:
	/// player class which ownes this object
	Player* m_pOwner;

	/// cell's current health points from the interval [0 ... 100]
	float m_fHealthPoints;
};