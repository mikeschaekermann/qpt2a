#pragma once

#include "../managers/ScreenManager.h"
#include "../input/TouchWay.h"
#include "GUIItem.h"

class ScreenManager;
class GUIItem;
class Screen
{
public:
	GUIItem* focusedItem;

	Screen();
	virtual ~Screen(void);

	virtual void update(float frameTime);
	virtual void draw() = 0;

	/**
		@brief event method called when a new touch has begun
		@param touchWay			touch way of the newly begun touch
	 */
	virtual bool touchBegan(const TouchWay & touchWay);
	/**
		@brief event method called when an existing touch has moved
		@param touchWay			touch way of the touch, concerned
	 */
	virtual void touchMoved(const TouchWay & touchWay) = 0;
	/**
		@brief event method called when the mouse has moved
		@param event			mouse event for mouse motion
	 */
	virtual bool mouseMove( MouseEvent event );
	/**
		@brief event method called when an existing touch has ended
		@param touchWay			touch way of the touch, concerned; must be called by copy,
								because touch way object will be destroyed after call to this method
	 */
	virtual void touchEnded(TouchWay touchWay);
	/**
		@brief event method called when an touch was released as a click (i.e. no drag!)
		@param touchWay			touch way of the touch, concerned; must be called by copy,
								because touch way object will be destroyed after call to this method
	 */
	virtual void touchClick(TouchWay touchWay) = 0;

	virtual void resize(ResizeEvent event) = 0;

	virtual void onKeyInput(KeyEvent& e);

protected:
	GUIItem* rootItem;
};