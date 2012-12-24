#define _USE_MATH_DEFINES

#include <iostream>
#include <boost/thread/thread.hpp>
#include "event/EventQueue.h"
#include "network/ServerNetworkManager.h"
#include "game/Game.h"
//#include "game\Game.h"

using namespace std;

int main(int argc, char argv[])
{
	int port;
	cin >> port;

	if(cin.fail())
	{
		cout << "Error: input failes" << endl;
	}
	
	Game game;

	EventQueue eq;
	
	ServerNetworkManager nm(port, 0);

	boost::thread networkThread(boost::bind(&NetworkManager::operator(), &nm));
	//boost::thread eventQueueThread(boost::bind(&EventQueue::operator(), &eq));
	game.bind(&nm, &eq);
	networkThread.join();
	//eventQueueThread.join();

	cout << "test";
	return 0;
}

