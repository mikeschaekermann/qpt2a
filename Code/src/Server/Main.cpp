#define _USE_MATH_DEFINES

#include <iostream>
#include <boost\thread\thread.hpp>
#include "game\Game.h"
#include "network/ServerNetworkManager.h"
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

	
	// add game to networkmanager
	// networkmanager singleton??
	// access from game object to networkmanager??
	ServerNetworkManager nm(port, &game);

	boost::thread thr(boost::bind(&NetworkManager::operator(), &nm));
	
	game.bindNetworkManager(&nm);

	thr.join();
	cout << "test";
	return 0;
}

