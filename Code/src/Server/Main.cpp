#define _USE_MATH_DEFINES

#include <iostream>
#include <boost\thread\thread.hpp>

#include "network\NetworkManager.h"
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

	//Game game;
	// add game to networkmanager
	// networkmanager singleton??
	// access from game object to networkmanager??
	NetworkManager nm(port);
	boost::thread networkThread(nm);

	cout << "test";
	return 0;
}

