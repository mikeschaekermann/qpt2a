#pragma once

#include "../../common/ICollisionBehavior.h"

class GameObject : public ICollisionBehavior
{
public:
	bool intersects() const
	{
		return m_pCollisionBehavior->intersects();
	}
protected:
	ICollisionBehavior* m_pCollisionBehavior;
};