#include "Screen.h"
#include "ScreenManager.h"

Screen::Screen(ScreenManager& screenManager) :
	m_screenManager(screenManager),
	m_screenState(ScreenState::TransitionOn),
	m_fTransitionOnTime(1),
	m_fTransitionOffTime(1),
	m_bIsInitialized(false),
	m_bCoveredByOtherScreen(false),
	m_bIsExiting(false)
{
}

Screen::~Screen()
{
}

void Screen::update(float frameTime)
{
	/*if(m_bIsExiting)
	{
		m_screenState = ScreenState::TransitionOff;

		if(!updateTransition(frameTime, m_fTransitionOffTime))
		{
			ScreenManager::getInstance()->closeScreen();
		}
	}
	else
	{
		if(updateTransition(frameTime, m_fTransitionOnTime))
		{

		}
		else
		{

		}
	}*/
}

bool Screen::updateTransition(float frameTime, float time)
{
	return true;
}