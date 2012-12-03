#include "..\Client\GameManager.h"

GameManager* const GameManager::m_pManager = new GameManager();

GameManager::GameManager(void)
{
}

GameManager::~GameManager(void)
{
	delete m_pScene;
}