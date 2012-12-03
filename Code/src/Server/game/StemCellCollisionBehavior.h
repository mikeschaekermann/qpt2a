#pragma once

#include "ICollisionBehavior.h"

class StemCellCollisionBehavior : public ICollisionBehavior
{
public:
	virtual bool intersects() const
	{
		return false;
	}
};