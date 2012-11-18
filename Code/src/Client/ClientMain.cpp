#pragma once

#include "ClientMain.h"
#include "Config.h"

void ClientMain::setup()
{
	setWindowPos(100, 100);

	Logger::getInstance()->configure("main.log");
	Logger::getInstance()->log(LogSeverity::LOG_INFO, "Test");
}

void ClientMain::update()
{
	
}

void ClientMain::draw()
{

}


CINDER_APP_BASIC( ClientMain, RendererGl )