#pragma once
class IMotionBehavior
{
public:
	virtual ~IMotionBehavior(void) = 0;
	void virtual update(float frameTime) = 0;
};