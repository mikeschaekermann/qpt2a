#pragma once

#include "ClientMain.h"
#include "../common/Logger.h"
#include "../common/Config.h"
#include "GameManager.h"

void ClientMain::setup()
{
	setWindowPos(100, 100);

	LOG_INFO("\n\n\n");
	LOG_INFO("Client start up");

	m_fFrameTime = 0;
	m_fElapsedGameTimeLastFrame = getElapsedSeconds();
}

void ClientMain::update()
{
	m_fFrameTime = getElapsedSeconds() - m_fElapsedGameTimeLastFrame;

	

	m_fElapsedGameTimeLastFrame = getElapsedSeconds();
}

void ClientMain::draw()
{
	
}


CINDER_APP_BASIC( ClientMain, RendererGl )