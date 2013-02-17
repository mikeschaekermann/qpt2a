#pragma once

#include "ClientMain.h"
#include "../common/Logger.h"
#include "../common/Config.h"
#include "managers/GameManager.h"
#include "managers/ScreenManager.h"
#include "managers/AssetManager.h"
#include "../common/ConfigurationDataHandler.h"

void ClientMain::setup()
{
	setWindowPos(100, 100);
	
	srand((unsigned int) time(0));
	m_fFrameTime = 0;
	m_fElapsedGameTimeLastFrame = (float)getElapsedSeconds();

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

	ConfigurationDataHandler::getInstance()->readFromXML("..\\..\\config.xml");
	AssetManager::getInstance()->loadAssets("..\\..\\..\\resources\\assets.xml");
}

void ClientMain::shutdown()
{
	AssetManager::releaseInstance();
	GameManager::releaseInstance();
	ScreenManager::releaseInstance();
	exit(0);
}

void ClientMain::prepareSettings( Settings *settings )
{
	settings->enableMultiTouch();
	settings->setWindowSize(1024, 768);
	settings->setResizable(false);
}

void ClientMain::update()
{
	m_fFrameTime = (float)getElapsedSeconds() - m_fElapsedGameTimeLastFrame;

	GAME_MGR->update(m_fFrameTime);

	m_fElapsedGameTimeLastFrame = (float)getElapsedSeconds();
}

void ClientMain::draw()
{
	gl::clear(Color(0.7f, 0.9f, 0.9f));
	GAME_MGR->draw();
	gl::drawString(toString(getAverageFps()), Vec2f(10, 10), Color::white());
}

void ClientMain::mouseDown( MouseEvent event )
{
	TouchWay::TRIGGER trigger;
	if (event.isLeftDown())
	{
		trigger = TouchWay::LEFT;
	}
	else if (event.isRightDown())
	{
		trigger = TouchWay::RIGHT;
	}
	else if (event.isMiddleDown())
	{
		trigger = TouchWay::MIDDLE;
	}
	m_touchWays.insert(make_pair(-1, TouchWay(-1, event.getPos(), m_fElapsedGameTimeLastFrame, trigger)));
	SCREEN_MGR->touchBegan(m_touchWays[-1]);
}

void ClientMain::mouseDrag( MouseEvent event )
{
	m_touchWays[-1].addPoint(event.getPos(), m_fElapsedGameTimeLastFrame);
	SCREEN_MGR->touchMoved(m_touchWays[-1]);
}

void ClientMain::mouseUp( MouseEvent event )
{
	m_touchWays[-1].addPoint(event.getPos(), m_fElapsedGameTimeLastFrame);
	SCREEN_MGR->touchEnded(m_touchWays[-1]);
	m_touchWays.erase(-1);
}

void ClientMain::mouseMove( MouseEvent event )
{
	SCREEN_MGR->mouseMove(event);
}

void ClientMain::mouseWheel( MouseEvent event )
{
	SCREEN_MGR->mouseWheel(event);
}

void ClientMain::touchesBegan( TouchEvent event )
{
	for (auto touchIt = event.getTouches().begin(); touchIt != event.getTouches().end(); ++touchIt)
	{
		m_touchWays.insert(make_pair(touchIt->getId(), TouchWay(touchIt->getId(), touchIt->getPos(), m_fElapsedGameTimeLastFrame, TouchWay::FINGER)));
		SCREEN_MGR->touchBegan(m_touchWays[touchIt->getId()]);
	}
}

void ClientMain::touchesMoved( TouchEvent event )
{
	for (auto touchIt = event.getTouches().begin(); touchIt != event.getTouches().end(); ++touchIt)
	{
		m_touchWays[touchIt->getId()].addPoint(touchIt->getPos(), m_fElapsedGameTimeLastFrame);
		SCREEN_MGR->touchMoved(m_touchWays[touchIt->getId()]);
	}
}

void ClientMain::touchesEnded( TouchEvent event )
{
	for (auto touchIt = event.getTouches().begin(); touchIt != event.getTouches().end(); ++touchIt)
	{
		m_touchWays[touchIt->getId()].addPoint(touchIt->getPos(), m_fElapsedGameTimeLastFrame);
		SCREEN_MGR->touchMoved(m_touchWays[touchIt->getId()]);
		m_touchWays.erase(touchIt->getId());
	}
}

void ClientMain::keyDown( KeyEvent event )
{
	SCREEN_MGR->onKeyInput(event);
}

void ClientMain::resize( ResizeEvent event )
{
	SCREEN_MGR->resize(event);
}

CINDER_APP_BASIC( ClientMain, RendererGl )