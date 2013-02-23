/*
	Copyright (c) 2013 Daniel Margreiter, Daniel Satanik, Bernhard Öhlinger, Mike Schäkermann, Barbara Sterflinger, Sebastian Drack, Thomas Pichler, Philipp Födermayr, Wilfried Gruber
	Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
	The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
	THIS PROJECT WAS CONCEIVED AND IMPLEMENTED BY
	STUDENTS OF THE UNIVERSITY OF APPLIED SCIENCES SALZBURG
	FROM THE DEPARTMENTS OF MULTIMEDIA TECHNOLOGY
	AND MULTIMEDIA ART AS A QUALIFICATIONAL SEMESTER PROJECT.
 */

#define _USE_MATH_DEFINES

#include <iostream>
#include <boost/thread/thread.hpp>

#include "event/EventManager.h"

#include "game/GameContext.h"
#include "game/Game.h"

#include "../common/network/NetworkManager.h"

#include "../common/Config.h"

using namespace std;

int main(int argc, char argv[])
{
	try
	{
		int port = 2345;

		ConfigurationDataHandler::getInstance()->readFromXML("..\\..\\config.xml");

		Game game;

		GAMECONTEXT->initializeNetworkManager(port, &game);

		boost::thread networkThread(boost::bind(&NetworkManager::operator(), NETWORKMANAGER));
		boost::thread eventQueueThread(boost::bind(&EventManager::operator(), EVENT_MGR));

		networkThread.join();
		eventQueueThread.join();

		cout << "Application over";
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

