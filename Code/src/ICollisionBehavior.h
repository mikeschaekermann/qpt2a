#pragma once
class ICollisionBehavior
{
public:
	virtual ~ICollisionBehavior(void) = 0;
	void virtual update(float frameTime) = 0;
};

