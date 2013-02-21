#pragma once

#include "../../common/Config.h"
#include "../screens/Screen.h"
#include <stack>
#include "../input/TouchWay.h"
#include "boost/thread/mutex.hpp"

#define SCREEN_MGR ScreenManager::getInstance()
#define CONN_SCR SCREEN_MGR->getConnectScreen()
#define GAME_SCR SCREEN_MGR->getGameScreen()

class Screen;
class GameScreen;
class MenuScreen;
class ConnectScreen;
class CreditsScreen;
class IntroScreen;
class ScreenManager
{
public:
	~ScreenManager(void);

	void update(float frameTime);
	void draw();

	void openScreen();
	void openMenuScreen();
	void openGameScreen();
	void openConnectScreen();
	void openCreditsScreen();
	void openIntroScreen();
	GameScreen * getGameScreen() const;
	ConnectScreen * getConnectScreen() const;
	void closeScreen();
	void exit();
	void fadeToBlack(float alpha);

	void onKeyInput(KeyEvent& e) const;

	void mouseWheel(MouseEvent & e) const;

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
		@brief event method called when the mouse has moved
		@param event			mouse event for mouse motion
	 */
	virtual void mouseMove( MouseEvent event );
	/**
		@brief event method called when an existing touch has ended
		@param touchWay			touch way of the touch, concerned; must be called by copy,
								because touch way object will be destroyed after call to this method
	 */
	void touchEnded(TouchWay touchWay);

	void resize(ResizeEvent event);

	void refreshGameScreen();

	static ScreenManager * getInstance();
	static void releaseInstance()
	{
		instanceMutex.lock();
		if (instance != nullptr)
		{
			delete instance;
			instance = nullptr;
		}
		instanceMutex.unlock();
	}
private:
	ScreenManager();
	ScreenManager(const ScreenManager &) {}
	static ScreenManager * instance;
	static boost::mutex instanceMutex;

	//std::stack<Screen*> m_screenStack;
	Screen* m_backgroundScreen;
	MenuScreen * menuScreen;
	GameScreen * gameScreen;
	ConnectScreen* connectScreen;
	CreditsScreen* creditsScreen;
	IntroScreen* introScreen;
	Screen * currentScreen;
};