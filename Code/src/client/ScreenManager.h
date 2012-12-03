#pragma once

#include "../common/Config.h"
#include "Screen.h"
#include <stack>

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

private:
	std::stack<Screen*> m_screenStack;
	Screen* m_backgroundScreen;
};

