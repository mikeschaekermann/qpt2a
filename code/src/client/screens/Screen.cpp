#include "Screen.h"
#include "../managers/ScreenManager.h"

Screen::Screen() :
	screenState(ScreenState::TransitionOn),
	transitionOnTime(1),
	transitionOffTime(1),
	isInitialized(false),
	coveredByOtherScreen(false),
	isExiting(false),
	rootItem(nullptr)
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

void Screen::draw()
{
	gl::enableAlphaBlending();
	gl::color(ColorA(1, 1, 1, 1));

	rootItem.draw();
}

bool Screen::updateTransition(float frameTime, float time)
{
	return true;
}

void Screen::mouseMove( MouseEvent event )
{
	rootItem.isMouseOverItem(event.getPos());
}

void Screen::touchBegan(const TouchWay & touchWay)
{
	rootItem.isMouseDownOnItem(touchWay.getCurrentPos());
}

void Screen::touchEnded(TouchWay touchWay)
{
	rootItem.isMouseUp();
}