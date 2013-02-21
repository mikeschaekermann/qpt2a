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
#include "../../common/environment/StaticModificator.h"

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
	void addStaticModifier(unsigned int id, Vec3f position, Vec3f rotation, Vec3f scale, float radius, StaticModificator::Type type);
	void addBarrier(unsigned int id, Vec3f position, Vec3f rotation, Vec3f scale, float radius);
	void addPlayer(unsigned int id, string name, unsigned int stemCellId, Vec3f stemCellPosition);
	void setMyPlayerId(unsigned int id);
	PlayerClient * getMyPlayer() const;
	float getMyHue() const;
	PlayerClient * getPlayerById(unsigned int id) const;
	float getHueByPlayerId(unsigned int id) const;
	map<unsigned int, PlayerClient*> const & getPlayers() const;
	
	ClientNetworkManager * getNetworkManager();
	boost::asio::ip::udp::endpoint getServerEndpoint();

	bool isInDebugMode() const { return debugMode; }
	void toggleDebugMode() { debugMode = !debugMode; }

	void quit();
private:
	/// singleton instance
	static GameManager* m_pManager;
	static boost::mutex	instanceMutex;

	/// id map for all active players
	map<unsigned int, PlayerClient*> players;

	/// flag indicating whether game is running in debug mode or not
	bool debugMode;

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

