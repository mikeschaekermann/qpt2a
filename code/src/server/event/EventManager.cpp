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
	mutex.lock();
	toAddList.push_back(e);
	mutex.unlock();
}

void EventManager::operator()()
{
	while (run)
	{
		boost::this_thread::sleep(boost::posix_time::milliseconds(33));
		while (!events.empty() && events.top()->getDeadTime() < timer.getSeconds())
		{
			GAMECONTEXT->getMutex().lock();
			events.top()->trigger();
			GAMECONTEXT->getMutex().unlock();
			delete events.top();
			events.pop();
		}
		mutex.lock();
		for (auto it = toAddList.begin(); it != toAddList.end(); ++it)
		{
			events.push(*it);
		}
		toAddList.clear();
		mutex.unlock();
	}
}

EventManager::EventManager() :
	run(true)
{
	timer.start();
}

EventManager::EventManager(const EventManager & cpy) { }

void EventManager::operator=(const EventManager & cpy) {  }