#pragma once

#include "ClientMain.h"
#include "../common/Config.h"
#include "network/ClientNetworkManager.h"
#include "boost/asio.hpp"
#include "../common/network/messages/game/outgame/JoinRequest.h"

void ClientMain::setup()
{
	setWindowPos(100, 100);

	Logger::getInstance()->configure("main.log");
	LOG_INFO("\n\n\n");
	LOG_INFO("Client start up");

	boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::address_v4::loopback(), 2345);
	ClientNetworkManager nm(endpoint);

	boost::thread thr(boost::bind(&NetworkManager::operator(), &nm));

	JoinRequest request;
	request.endpoint = endpoint;
	string test = "ABCD";
	request.name = test.c_str();
	request.nameSize = test.size();

	nm.send(request);
	
}

void ClientMain::update()
{
	
}

void ClientMain::draw()
{

}


CINDER_APP_BASIC( ClientMain, RendererGl )