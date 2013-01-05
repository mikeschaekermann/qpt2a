#pragma once

#include "ClientMain.h"
#include "../common/Logger.h"
#include "../common/Config.h"
#include "managers/GameManager.h"

void ClientMain::setup()
{
	setWindowPos(100, 100);

	m_fFrameTime = 0;
	m_fElapsedGameTimeLastFrame = getElapsedSeconds();

	Logger::getInstance()->configure("main.log");

	LOG_INFO("\n\n\n");
	LOG_INFO("Client start up");
	
	LOG_INFO("PROGRAM START");

	if (System::hasMultiTouch())
	{
		LOG_INFO("The current environment does support multi-touch events.");
		LOG_INFO("The maximum number of parallel multi-touch points is: " + System::getMaxMultiTouchPoints());
	}
	else
	{
		LOG_INFO("The current environment does not support multi-touch events.");
	}

	GAME_MGR->startGame("Mike");
}

void ClientMain::prepareSettings( Settings *settings )
{
	settings->enableMultiTouch();
}

void ClientMain::update()
{
	m_fFrameTime = getElapsedSeconds() - m_fElapsedGameTimeLastFrame;

	GameManager::getInstance()->update(m_fFrameTime);

	m_fElapsedGameTimeLastFrame = getElapsedSeconds();
}

void ClientMain::draw()
{
	GameManager::getInstance()->draw();
}

void ClientMain::mouseDown( MouseEvent event )
{
	m_touchWays.insert(make_pair(-1, TouchWay(-1, event.getPos(), m_fElapsedGameTimeLastFrame)));
	GAME_MGR->getScreenManager().touchBegan(m_touchWays[-1]);
}

void ClientMain::mouseDrag( MouseEvent event )
{
	m_touchWays[-1].addPoint(event.getPos(), m_fElapsedGameTimeLastFrame);
	GAME_MGR->getScreenManager().touchMoved(m_touchWays[-1]);
}

void ClientMain::mouseUp( MouseEvent event )
{
	m_touchWays[-1].addPoint(event.getPos(), m_fElapsedGameTimeLastFrame);
	GAME_MGR->getScreenManager().touchEnded(m_touchWays[-1]);
	m_touchWays.erase(-1);
}

void ClientMain::touchesBegan( TouchEvent event )
{
	for (auto touchIt = event.getTouches().begin(); touchIt != event.getTouches().end(); ++touchIt)
	{
		m_touchWays.insert(make_pair(touchIt->getId(), TouchWay(touchIt->getId(), touchIt->getPos(), m_fElapsedGameTimeLastFrame)));
		GAME_MGR->getScreenManager().touchBegan(m_touchWays[touchIt->getId()]);
	}
}

void ClientMain::touchesMoved( TouchEvent event )
{
	for (auto touchIt = event.getTouches().begin(); touchIt != event.getTouches().end(); ++touchIt)
	{
		m_touchWays[touchIt->getId()].addPoint(touchIt->getPos(), m_fElapsedGameTimeLastFrame);
		GAME_MGR->getScreenManager().touchMoved(m_touchWays[touchIt->getId()]);
	}
}

void ClientMain::touchesEnded( TouchEvent event )
{
	for (auto touchIt = event.getTouches().begin(); touchIt != event.getTouches().end(); ++touchIt)
	{
		m_touchWays[touchIt->getId()].addPoint(touchIt->getPos(), m_fElapsedGameTimeLastFrame);
		GAME_MGR->getScreenManager().touchMoved(m_touchWays[touchIt->getId()]);
		m_touchWays.erase(touchIt->getId());
	}
}

void ClientMain::keyDown( KeyEvent event )
{

}

CINDER_APP_BASIC( ClientMain, RendererGl )