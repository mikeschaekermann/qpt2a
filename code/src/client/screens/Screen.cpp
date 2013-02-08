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
	gl::color(ColorA(1, 1, 1, 1));

	rootItem->draw();
}

void Screen::mouseMove( MouseEvent event )
{
	rootItem->isMouseOverItem(event.getPos());
}

void Screen::touchBegan(const TouchWay & touchWay)
{
	if(!rootItem->isMouseDownOnItem(touchWay.getCurrentPos()))
	{
		focusedItem = nullptr;
	}
}

void Screen::touchEnded(TouchWay touchWay)
{
	rootItem->isMouseUp();
}

void Screen::onKeyInput(KeyEvent& e)
{
	if(focusedItem)
	{
		focusedItem->onKeyInput(e);
	}
}