#define _USE_MATH_DEFINES

#include <iostream>
#include <boost/thread/thread.hpp>
#include "event/EventManager.h"
#include "network/ServerNetworkManager.h"
#include "game/Game.h"

using namespace std;

int main(int argc, char argv[])
{
	try
	{
		int port = 2345;
		//cin >> port;
	
		Game game;

		ServerNetworkManager nm(port, &game);
		boost::thread networkThread(boost::bind(&NetworkManager::operator(), &nm));
		boost::thread eventQueueThread(boost::bind(&EventManager::operator(), EVENT_MGR));
		game.bind(&nm);
		networkThread.join();
		eventQueueThread.join();

		cout << "test";
		return 0;
	}
	catch(exception & e)
	{
		LOG_ERROR(e.what());
		return -1;
	}
	catch(string & e)
	{
		LOG_ERROR(e);
		return -1;
	}
}

