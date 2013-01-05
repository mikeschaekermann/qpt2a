#include "GameManager.h"
#include "boost/asio.hpp"
#include "../../common/network/messages/game/outgame/JoinRequest.h"
#include "../../client/actors/PlayerClient.h"

GameManager* GameManager::m_pManager = nullptr;

GameManager::GameManager(void):
	serverEndpoint(boost::asio::ip::address_v4::loopback(), 2345)
{
	networkManager = new ClientNetworkManager(serverEndpoint);
	boost::thread(boost::bind(&NetworkManager::operator(), networkManager));

	m_screenManager.openScreen(new GameScreen(m_screenManager));
}

GameManager::~GameManager(void)
{
	for (auto it = players.begin(); it != players.end(); ++it)
	{
		if (*it != nullptr)
		{
			delete *it;
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

void GameManager::addPlayer(unsigned int id, string name, Vec2f position, bool canManipulate)
{
	auto newPlayer = new PlayerClient(id, name, canManipulate);
	
	for (auto it = players.begin(); it != players.end(); ++it)
	{
		if (**it == *newPlayer) return;
	}

	players.push_back(newPlayer);
}

void GameManager::setMyPlayerId(unsigned int id)
{
	myPlayer->setId(id);
}