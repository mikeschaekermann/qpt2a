#include "ScreenManager.h"

ScreenManager::ScreenManager(void):
	m_backgroundScreen(nullptr)
{
}

ScreenManager::~ScreenManager(void)
{
	while (!m_screenStack.empty())
	{
		auto tmp = m_screenStack.top();
		m_screenStack.pop();
		delete tmp;
	}
}

void ScreenManager::update(float frameTime)
{
	m_screenStack.top()->update(frameTime);
	
	if(m_backgroundScreen)
	{
		m_backgroundScreen->update(frameTime);
	}
}

void ScreenManager::draw()
{
	m_screenStack.top()->draw();

	if(m_backgroundScreen)
	{
		m_backgroundScreen->draw();
	}
}

void ScreenManager::openScreen(Screen* screen)
{
	screen->loadContent();

	m_screenStack.push(screen);
}

void ScreenManager::closeScreen()
{
	m_screenStack.pop();
}

void ScreenManager::fadeToBlack(float alpha)
{
	auto windowSize = getWindowSize();
	color(cinder::ColorA(0, 0, 0, alpha));
	drawSolidRect(cinder::Rectf(0, 0, windowSize.x, windowSize.y));
}