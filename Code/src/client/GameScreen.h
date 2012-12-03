#pragma once

#include "Screen.h"
#include "../common/Config.h"
#include "../common/GameObject.h"
#include "../common/Player.h"

class GameScreen :
	public Screen
{
public:
	GameScreen(ScreenManager& screenManager);
	virtual ~GameScreen(void);

	virtual void update(float frameTime);
	virtual void draw();

	virtual void loadContent();

private:
	/// scene object
	GameObject* m_pScene;

	/// all active players
	vector<Player*> m_players;
};

