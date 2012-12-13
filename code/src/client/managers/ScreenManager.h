#pragma once

#include "../../common/Config.h"
#include "../screens/Screen.h"
#include <stack>
#include "../input/TouchWay.h"

class Screen;

class ScreenManager
{
public:
	ScreenManager(void);
	~ScreenManager(void);

	void update(float frameTime);
	void draw();

	void openScreen(Screen* screen);
	void closeScreen();
	void exit();
	void fadeToBlack(float alpha);

	/**
		@brief event method called when a new touch has begun
		@param touchWay			touch way of the newly begun touch
	 */
	void touchBegan(const TouchWay & touchWay);
	/**
		@brief event method called when an existing touch has moved
		@param touchWay			touch way of the touch, concerned
	 */
	void touchMoved(const TouchWay & touchWay);
	/**
		@brief event method called when an existing touch has ended
		@param touchWay			touch way of the touch, concerned; must be called by copy,
								because touch way object will be destroyed after call to this method
	 */
	void touchEnded(TouchWay touchWay);

private:
	std::stack<Screen*> m_screenStack;
	Screen* m_backgroundScreen;
};

