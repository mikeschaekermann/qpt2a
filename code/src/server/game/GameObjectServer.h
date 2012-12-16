#pragma once

#include "../../common/GameObject.h"
#include "IIdentifiable.h"

class GameObjectServer : 
	virtual public GameObject,
	public IIdentifiable
{
public:
	bool intersects() const
	{
		return m_pCollisionBehavior->intersects();
	}
protected:
	ICollisionBehavior* m_pCollisionBehavior;
};