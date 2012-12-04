#pragma once

#include "ClientMain.h"
#include "../common/Logger.h"
#include "../common/Config.h"
#include "GameManager.h"
#include "network/ClientNetworkManager.h"
#include "boost/asio.hpp"
#include "../common/network/messages/game/outgame/JoinRequest.h"

void	ClientMain::setup()
{
	setWindowPos(100, 100);

	m_fFrameTime = 0;
	m_fElapsedGameTimeLastFrame = getElapsedSeconds();

	Logger::getInstance()->configure("main.log");
	
	LOG_INFO("\n\n\n");
	LOG_INFO("Client start up");

	/*boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::address_v4::loopback(), 2345);
	ClientNetworkManager nm(endpoint);

	boost::thread thr(boost::bind(&NetworkManager::operator(), &nm));

	JoinRequest request;
	request.endpoint = endpoint;
	string test = "ABCD";
	request.name = test.c_str();
	request.nameSize = test.size();

	nm.send(request);*/
	
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
}

void	ClientMain::prepareSettings( Settings *settings )
{
	settings->enableMultiTouch();
}

void	ClientMain::update()
{
	m_fFrameTime = getElapsedSeconds() - m_fElapsedGameTimeLastFrame;

	GameManager::getInstance()->update(m_fFrameTime);

	m_fElapsedGameTimeLastFrame = getElapsedSeconds();
}

void	ClientMain::draw()
{
	GameManager::getInstance()->draw();
}

void	ClientMain::mouseDown( MouseEvent event )
{

}

void	ClientMain::mouseDrag( MouseEvent event )
{

}

void	ClientMain::touchesBegan( TouchEvent event )
{

}

void	ClientMain::touchesMoved( TouchEvent event )
{
	
}

void	ClientMain::touchesEnded( TouchEvent event )
{

}

void	ClientMain::keyDown( KeyEvent event )
{

}

CINDER_APP_BASIC( ClientMain, RendererGl )