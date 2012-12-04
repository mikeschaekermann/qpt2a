#pragma once

#include "ScreenManager.h"

enum ScreenState
{
	TransitionOn,
	Active,
	TransitionOff,
	Hidden
};

class ScreenManager;
class Screen
{
public:
	Screen(ScreenManager& screenManager);
	virtual ~Screen(void);

	virtual void update(float frameTime);
	virtual void draw() = 0;

	virtual void loadContent() = 0;

protected:
	float m_fTransitionOnTime;
	float m_fTransitionOffTime;

	bool m_bIsInitialized;
	bool m_bCoveredByOtherScreen;

	ScreenState m_screenState;

private:
	bool m_bIsExiting;

	ScreenManager& m_screenManager;

	bool updateTransition(float frameTime, float time);
};