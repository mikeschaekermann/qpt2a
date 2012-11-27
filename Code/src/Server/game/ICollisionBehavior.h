#pragma once

class ICollisionBehavior
{
public:
	virtual bool intersects() const = 0;
};