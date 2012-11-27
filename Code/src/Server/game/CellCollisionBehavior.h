#pragma once

#include "ICollisionBehavior.h"

class CellCollisionBehavior : public ICollisionBehavior
{
public:
	virtual bool intersects() const
	{
		return false;
	}
};