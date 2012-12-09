#include "..\Client\GameManager.h"

GameManager* const GameManager::m_pManager = new GameManager();

GameManager::GameManager(void)
{
	m_screenManager.openScreen(new GameScreen(m_screenManager));
}

GameManager::~GameManager(void)
{

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