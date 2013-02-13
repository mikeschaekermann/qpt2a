#include "boost/thread/thread.hpp"

#include "EventManager.h"
#include "GameEvent.h"
#include "../game/GameContext.h"

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
	GAMECONTEXT->getMutex().lock();
	toAddList.push_back(e);
	GAMECONTEXT->getMutex().unlock();
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
		GAMECONTEXT->getMutex().lock();
		for (auto it = toAddList.begin(); it != toAddList.end(); ++it)
		{
			events.push(*it);
		}
		toAddList.clear();
		GAMECONTEXT->getMutex().unlock();
	}
}

EventManager::EventManager() :
	run(true)
{
	timer.start();
}

EventManager::EventManager(const EventManager & cpy) { }

void EventManager::operator=(const EventManager & cpy) {  }