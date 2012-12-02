#pragma once

#include <queue>

#include "Event.h"

using namespace std;

class EventQueue
{
public:
	void addEvent(Event* e)
	{
		e->setDeadTime(m_fTime);
		m_events.push(e);
	}

	void operator()()
	{
		if (m_events.top()->getDeadTime() < m_fTime)
		{
			m_events.top()->trigger();
			delete m_events.top();
			m_events.pop();
		}
	}
private:
	float m_fTime;
	priority_queue<Event*> m_events;
};