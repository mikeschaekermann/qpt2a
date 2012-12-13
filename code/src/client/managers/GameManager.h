#pragma once

#include <stack>
#include "../../common/GameObject.h"
#include "../screens/Screen.h"
#include "../screens/GameScreen.h"
#include "ScreenManager.h"
#include "../../common/network/NetworkManager.h"

#define GAME_MGR GameManager::getInstance()

class GameManager
{
public:
	~GameManager(void);

	static GameManager * const getInstance();

	void update(float frameTime);
	void draw();
	ScreenManager & getScreenManager();

private:
	/// singleton instance
	static GameManager* m_pManager;

	ScreenManager m_screenManager;

	/// game version
	string m_gameVersion;

	GameManager(void);
	GameManager(const GameManager&);
};

