#pragma once

#include <queue>

#include "boost/thread/thread.hpp"
#include "cinder/Timer.h"

#include "Event.h"

using namespace std;

class EventQueue
{
public:
	EventQueue() :
	  m_bRun(true)
	{
		m_timer.start();
	}

	double getTime()
	{
		return m_timer.getSeconds();
	}

	void addEvent(GameEvent * e)
	{
		m_events.push(e);
	}

	void operator()()
	{
		while (m_bRun)
		{
			if (m_events.top()->getDeadTime() < m_timer.getSeconds())
			{
				m_events.top()->trigger();
				delete m_events.top();
				m_events.pop();
			}
			else
			{
				boost::this_thread::sleep(boost::posix_time::milliseconds(33));
			}
		}
	}
private:
	ci::Timer m_timer;
	bool m_bRun;
	priority_queue<GameEvent *> m_events;
};