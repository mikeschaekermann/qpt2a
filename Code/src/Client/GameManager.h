#pragma once

#include <stack>
#include "..\GameObject.h"
#include "IMenu.h"

class GameManager
{
public:
	~GameManager(void);

	GameManager * const getInstance() { return m_pManager; }

private:
	/// singleton instance
	static GameManager* const m_pManager;
	/// scene object
	GameObject* m_pScene;

	/// current menu stack
	std::stack<IMenu> m_menuStack;
	/// all active players
	vector<Player*> m_players;

	/// game version
	string m_gameVersion;

	GameManager(void);
};

