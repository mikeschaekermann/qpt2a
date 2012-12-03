#pragma once

#include <stack>
#include "..\common\GameObject.h"
#include "Screen.h"
#include "GameScreen.h"
#include "ScreenManager.h"
#include "../common/network/NetworkManager.h"

class GameManager
{
public:
	~GameManager(void);

	static GameManager * const getInstance() { return m_pManager; }

	void update(float frameTime);
	void draw();

private:
	/// singleton instance
	static GameManager* const m_pManager;

	ScreenManager m_screenManager;

	/// game version
	string m_gameVersion;

	GameManager(void);
};

