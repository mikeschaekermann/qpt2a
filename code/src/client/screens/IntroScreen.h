#pragma once

#include "screen.h"
#include "cinder/qtime/QuickTime.h"

using namespace ci::qtime;

class IntroScreen :
	public Screen
{
public:
	IntroScreen(void);
	virtual ~IntroScreen(void);
	
	virtual void update(float frameTime);
	virtual void draw();

	virtual void resize();

private:
	bool isRunning;
	MovieGl movie;
};

