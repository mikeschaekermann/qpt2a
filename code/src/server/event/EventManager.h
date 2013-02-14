#pragma once

class GameEvent;

#include <queue>
#include <vector>
#include <boost\thread\mutex.hpp>

#include "cinder/Timer.h"

#define EVENT_MGR EventManager::getInstance()

class EventManager
{
public:
	static EventManager * getInstance();

	double getTime() const;

	void operator+=(GameEvent * e);

	void operator()();
private:
	EventManager();
	EventManager(const EventManager & cpy);
	void operator=(const EventManager & cpy);

	static EventManager * instance;

	ci::Timer timer;
	bool run;
	std::priority_queue<GameEvent *> events;
	std::vector<GameEvent *> toAddList;
	boost::mutex mutex;
};