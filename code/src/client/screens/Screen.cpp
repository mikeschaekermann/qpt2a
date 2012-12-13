#include "Screen.h"
#include "../managers/ScreenManager.h"

Screen::Screen(ScreenManager& screenManager) :
	screenManager(screenManager),
	screenState(ScreenState::TransitionOn),
	transitionOnTime(1),
	transitionOffTime(1),
	isInitialized(false),
	coveredByOtherScreen(false),
	isExiting(false)
{
}

Screen::~Screen()
{
}

void Screen::update(float frameTime)
{
	/*if(isExiting)
	{
		screenState = ScreenState::TransitionOff;

		if(!updateTransition(frameTime, transitionOffTime))
		{
			ScreenManager::getInstance()->closeScreen();
		}
	}
	else
	{
		if(updateTransition(frameTime, transitionOnTime))
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