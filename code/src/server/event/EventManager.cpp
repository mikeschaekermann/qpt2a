#include "boost/thread/thread.hpp"

#include "EventManager.h"
#include "GameEvent.h"

using namespace std;

EventManager * EventManager::instance = 0;

EventManager * EventManager::getInstance()
{
	if (instance == nullptr)
	{
		instance = new EventManager();
	}
	return instance;
}

double EventManager::getTime() const
{
	return timer.getSeconds();
}

void EventManager::operator+=(GameEvent * e)
{
	events.push(e);
}

void EventManager::operator()()
{
	while (run)
	{
		if (!events.empty() && events.top()->getDeadTime() < timer.getSeconds())
		{
			events.top()->trigger();
			delete events.top();
			events.pop();
		}
		else
		{
			boost::this_thread::sleep(boost::posix_time::milliseconds(33));
		}
	}
}

EventManager::EventManager() :
	run(true)
{
	timer.start();
}

EventManager::EventManager(const EventManager & cpy) { }

void EventManager::operator=(const EventManager & cpy) {  }