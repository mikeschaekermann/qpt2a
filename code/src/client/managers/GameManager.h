#pragma once

#include <stack>
#include "../../common/GameObject.h"
#include "../screens/Screen.h"
#include "../screens/MenuScreen.h"
#include "../screens/GameScreen.h"
#include "ScreenManager.h"
#include "../network/ClientNetworkManager.h"
#include "../actors/PlayerClient.h"
#include "boost/thread/mutex.hpp"

#define GAME_MGR GameManager::getInstance()
#define GAME_SCR SCREEN_MGR->getGameScreen()
#define NETWORK_MGR GAME_MGR->getNetworkManager()

class GameManager
{
public:
	~GameManager(void);

	static GameManager * const getInstance();
	static void releaseInstance() 
	{ 
		SCREEN_MGR->releaseInstance();
		
		instanceMutex.lock();
		if (m_pManager != nullptr)
		{
			delete m_pManager;
			m_pManager = nullptr;
		}
		instanceMutex.unlock();
	}

	void startGame(string playerName);
	void startGame(string playerName, string ip);
	void update(float frameTime);
	void draw();

	void addPetriDish(float radius);
	
	void addPlayer(unsigned int id, string name, unsigned int stemCellId, Vec3f stemCellPosition);
	void setMyPlayerId(unsigned int id);
	PlayerClient * getMyPlayer() const;
	PlayerClient * getPlayerById(unsigned int id);
	
	ClientNetworkManager * getNetworkManager();
	boost::asio::ip::udp::endpoint getServerEndpoint();

	void quit();
private:
	/// singleton instance
	static GameManager* m_pManager;
	static boost::mutex	instanceMutex;

	/// id map for all active players
	map<unsigned int, PlayerClient*> players;
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

