#include "Screen.h"
#include "../managers/ScreenManager.h"

Screen::Screen() :
	rootItem(new GUIItem(this, nullptr)),
	focusedItem(nullptr)
{
}

Screen::~Screen()
{
	delete rootItem;
}

void Screen::update(float frameTime)
{
}

void Screen::draw()
{
	gl::enableAlphaBlending();
	gl::clear(Color(1, 1, 1));
	gl::color(ColorA(1, 1, 1, 1));

	rootItem->draw();
}

bool Screen::mouseMove( MouseEvent event )
{
	return rootItem->isMouseOverItem(event.getPos());
}

bool Screen::touchBegan(const TouchWay & touchWay)
{
	if(!rootItem->isMouseDownOnItem(touchWay.getCurrentPos()))
	{
		focusedItem = nullptr;
		return false;
	}
	
	return true;
}

void Screen::touchMoved(const TouchWay & touchWay)
{
}


bool Screen::touchClick(TouchWay touchway)
{
	return rootItem->hasMouseClickedOnItem(touchway.getCurrentPos());
}


void Screen::touchEnded(TouchWay touchWay)
{
	rootItem->isMouseUp();
}

void Screen::resize(ResizeEvent event)
{
}

void Screen::onKeyInput(KeyEvent& e)
{
	if(focusedItem)
	{
		focusedItem->onKeyInput(e);
	}
}

void Screen::mouseWheel(MouseEvent & e)
{
}