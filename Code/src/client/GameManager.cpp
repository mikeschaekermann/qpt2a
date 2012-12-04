#include "..\Client\GameManager.h"

GameManager* GameManager::m_pManager = nullptr;

GameManager::GameManager(void)
{
	m_screenManager.openScreen(new GameScreen(m_screenManager));
}

GameManager::~GameManager(void)
{
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
	m_screenManager.update(frameTime);
}

void GameManager::draw()
{
	m_screenManager.draw();
}