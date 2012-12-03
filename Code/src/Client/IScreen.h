#pragma once
class IScreen
{
public:
	virtual ~IScreen(void) = 0;

	virtual void update(float frameTime) = 0;
	virtual void draw() = 0;
};