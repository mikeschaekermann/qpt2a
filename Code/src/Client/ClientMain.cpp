#pragma once

#include "ClientMain.h"
#include "Config.h"

void ClientMain::setup()
{
	setWindowPos(100, 100);

	Logger::getInstance()->configure("main.log");
	LOG_INFO("\n\n\n");
	LOG_INFO("Client start up");
}

void ClientMain::update()
{
	
}

void ClientMain::draw()
{

}


CINDER_APP_BASIC( ClientMain, RendererGl )