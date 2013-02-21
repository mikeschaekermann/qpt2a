#pragma once

#include "GameEvent.h"

#include <queue>
#include <vector>
#include <boost/thread/mutex.hpp>

#include "cinder/Timer.h"

#define EVENT_MGR EventManager::getInstance()

struct CompareGameEvent : public std::binary_function<GameEvent*, GameEvent*, bool>                                                                                       
{  
  bool operator()(const GameEvent* lhs, const GameEvent* rhs) const  
  {  
     return lhs->getDeadTime() > rhs->getDeadTime();  
  }  
};

class EventManager
{
public:
	static EventManager * getInstance();

	double getTime() const;
	bool isDeletedEvent(unsigned int id);
	boost::mutex & getMutex();

	void operator+=(GameEvent * e);

	void operator()();
private:
	EventManager();
	EventManager(const EventManager & cpy);
	void operator=(const EventManager & cpy);

	static EventManager * instance;

	ci::Timer timer;
	bool run;
	std::priority_queue<GameEvent *, std::vector<GameEvent *>, CompareGameEvent> events;
	std::vector<GameEvent *> toAddList;
	std::map<unsigned int, bool> isDeletedList;
	boost::mutex mutex;
};