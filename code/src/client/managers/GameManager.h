#pragma once

#include <stack>
#include "../../common/GameObject.h"
#include "../screens/Screen.h"
#include "../screens/GameScreen.h"
#include "ScreenManager.h"
#include "../network/ClientNetworkManager.h"
#include "../actors/PlayerClient.h"

#define GAME_MGR GameManager::getInstance()

class GameManager
{
public:
	~GameManager(void);

	static GameManager * const getInstance();

	void startGame(string playerName);
	void update(float frameTime);
	void draw();
	ScreenManager & getScreenManager();

	void addPlayer(unsigned int id, string name, Vec2f position, bool canManipulate);
	void setMyPlayerId(unsigned int id);

private:
	/// singleton instance
	static GameManager* m_pManager;

	ScreenManager m_screenManager;

	/// all active players
	vector<PlayerClient*> players;
	/// my own player
	PlayerClient* myPlayer;

	boost::asio::ip::udp::endpoint serverEndpoint;
	ClientNetworkManager * networkManager;
	boost::thread networkManagerThread;

	/// game version
	string m_gameVersion;

	GameManager(void);
	GameManager(const GameManager&);
};

