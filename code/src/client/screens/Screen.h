#pragma once

#include "../managers/ScreenManager.h"
#include "../input/TouchWay.h"

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

	/**
		@brief event method called when a new touch has begun
		@param touchWay			touch way of the newly begun touch
	 */
	virtual void touchBegan(const TouchWay & touchWay) = 0;
	/**
		@brief event method called when an existing touch has moved
		@param touchWay			touch way of the touch, concerned
	 */
	virtual void touchMoved(const TouchWay & touchWay) = 0;
	/**
		@brief event method called when an existing touch has ended
		@param touchWay			touch way of the touch, concerned; must be called by copy,
								because touch way object will be destroyed after call to this method
	 */
	virtual void touchEnded(TouchWay touchWay) = 0;
	/**
		@brief event method called when an touch was released as a click (i.e. no drag!)
		@param touchWay			touch way of the touch, concerned; must be called by copy,
								because touch way object will be destroyed after call to this method
	 */
	virtual void touchClick(TouchWay touchWay) = 0;

	virtual void resize(ResizeEvent event) = 0;
protected:
	float transitionOnTime;
	float transitionOffTime;

	bool isInitialized;
	bool coveredByOtherScreen;

	ScreenState screenState;

private:
	bool isExiting;

	ScreenManager& screenManager;

	bool updateTransition(float frameTime, float time);
};