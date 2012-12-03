#pragma once

#include "ClientMain.h"
#include "../common/Config.h"

void	ClientMain::setup()
{
	setWindowPos(100, 100);

	Logger::getInstance()->configure("main.log");

	LOG_INFO("\n\n\n");
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
	
}

void	ClientMain::draw()
{

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