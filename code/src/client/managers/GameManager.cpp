#include "GameManager.h"
#include "../../common/Config.h"
#include "boost/asio.hpp"
#include "../../common/network/messages/game/outgame/JoinRequest.h"
#include "../../client/actors/PlayerClient.h"
#include "../../client/actors/StemCellClient.h"

GameManager* GameManager::m_pManager = nullptr;

GameManager::GameManager(void):
	serverEndpoint(boost::asio::ip::address_v4::loopback(), 2345)
{
	networkManager = new ClientNetworkManager(serverEndpoint);
	boost::thread(boost::bind(&NetworkManager::operator(), networkManager));

	m_screenManager.openGameScreen(new GameScreen(m_screenManager));
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

	networkManager->stop();
	networkManagerThread.join();
	delete networkManager;
}

GameManager * const GameManager::getInstance()
{
	if(!m_pManager)
	{
		m_pManager = new GameManager();
	}

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

void GameManager::update(float frameTime)
{
	m_screenManager.update(frameTime);
}

void GameManager::draw()
{
	m_screenManager.draw();
}

ScreenManager & GameManager::getScreenManager()
{
	return m_screenManager;
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

	/// my own player was passed
	if (id == myPlayer->getId())
	{
		players.insert(make_pair(id, myPlayer));
	}
	/// another player was passed
	else
	{
		players.insert(make_pair(id, new PlayerClient(id, name, false)));
	}

	auto player = players[id];

	/// create stem cell
	StemCellClient * stemCell = new StemCellClient(stemCellId, Vec3f(0, 0, -30), 0);
	GAME_SCR.addGameObjectToPick(stemCell, true);
}

void GameManager::setMyPlayerId(unsigned int id)
{
	myPlayer->setId(id);
}