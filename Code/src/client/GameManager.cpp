#include "..\Client\GameManager.h"

GameManager* GameManager::m_pManager = nullptr;

GameManager::GameManager(void)
{
}

GameManager::~GameManager(void)
{
	delete m_pScene;
}

GameManager * const GameManager::getInstance()
{
	if(!m_pManager)
	{
		m_pManager = new GameManager();
	}

	return m_pManager;
}

void GameManager::update(float frameTime)
{

}

void GameManager::draw()
{

}