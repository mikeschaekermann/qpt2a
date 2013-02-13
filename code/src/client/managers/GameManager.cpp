#include "GameManager.h"
#include "../../common/Config.h"
#include "boost/asio.hpp"
#include "../../common/network/messages/game/outgame/JoinRequest.h"
#include "../../client/actors/PlayerClient.h"
#include "../../client/actors/StemCellClient.h"

#include "cinder/app/AppBasic.h"

using namespace ci;
using namespace ci::app;

GameManager* GameManager::m_pManager = nullptr;
boost::mutex GameManager::instanceMutex;

GameManager::GameManager(void):
	serverEndpoint(boost::asio::ip::address_v4::loopback(), 2345)
{
	networkManager = new ClientNetworkManager(serverEndpoint);
	boost::thread(boost::bind(&NetworkManager::operator(), networkManager));

	SCREEN_MGR->openMenuScreen();
}

GameManager::~GameManager(void)
{
	for (auto it = players.begin(); it != players.end(); ++it)
	{
		if (it->second != nullptr)
		{
			delete it->second;
		}
	}
/* IF UNCOMMENTED, THESE LINES PROVOKE AN EXCEPTION AT THE INITIALIZATION OF GAME MANAGER
	networkManager->stop();
	networkManagerThread.join();
	delete networkManager;
*/
}

GameManager * const GameManager::getInstance()
{
	instanceMutex.lock();
	if(!m_pManager)
	{
		m_pManager = new GameManager();
	}
	instanceMutex.unlock();

	return m_pManager;
}

void GameManager::startGame(string playerName)
{
	getInstance();

	JoinRequest *request = new JoinRequest();
	request->endpoint = serverEndpoint;
	request->name = playerName;
	networkManager->send(request);

	myPlayer = new PlayerClient(playerName, true);
}

void GameManager::startGame(string playerName, string ip)
{
	serverEndpoint = boost::asio::ip::udp::endpoint(boost::asio::ip::address_v4::from_string(ip.c_str()), serverEndpoint.port());

	startGame(playerName);
}

void GameManager::update(float frameTime)
{
	SCREEN_MGR->update(frameTime);
}

void GameManager::draw()
{
	SCREEN_MGR->draw();
}

void GameManager::addPetriDish(float radius)
{
	/*GameObjectClient * petriDish = new GameObjectClient();
	petriDish->setRadius(radius);
	petriDish->setScale(Vec3f(1, 1, 1 / radius / 2));
	petriDish->setPosition(Vec3f(0, 0, -1));
	GAME_SCR.addGameObjectToDraw(petriDish);
	GAME_SCR.setWorldRadius(radius);
	GAME_SCR.zoomToWorld();*/
}

void GameManager::addPlayer(unsigned int id, string name, unsigned int stemCellId, Vec3f stemCellPosition)
{
	if (players.find(id) != players.end())
	{
		stringstream message;
		message << "player id " << id << " is already in use!";
		LOG_ERROR(message.str());

		assert(false);
	}

	bool ownPlayerAdded = (id == myPlayer->getId());

	/// my own player was passed
	if (ownPlayerAdded)
	{
		players.insert(make_pair(id, myPlayer));
	}
	/// another player was passed
	else
	{
		players.insert(make_pair(id, new PlayerClient(id, name, false)));
	}

	auto player = players[id];
	StemCellClient * stemCell = new StemCellClient(stemCellId, stemCellPosition, 0, player);
	
	if (ownPlayerAdded)
	{
		GAME_SCR.addCellToPick(stemCell);
	}
	
	GAME_SCR.addGameObjectToDraw(stemCell);
	GAME_SCR.addGameObjectToCollide(stemCell);
}

void GameManager::setMyPlayerId(unsigned int id)
{
	myPlayer->setId(id);
}

PlayerClient * GameManager::getMyPlayer() const
{
	return myPlayer;
}

PlayerClient * GameManager::getPlayerById(unsigned int id)
{
	auto player = players.find(id);

	if (player != players.end())
	{
		return player->second;
	}
	else
	{
		return nullptr;
	}
}

ClientNetworkManager * GameManager::getNetworkManager()
{
	return networkManager;
}

boost::asio::ip::udp::endpoint GameManager::getServerEndpoint()
{
	return serverEndpoint;
}

void GameManager::quit()
{
	exit(0);
}