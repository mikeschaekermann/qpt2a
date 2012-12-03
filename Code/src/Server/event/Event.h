#pragma once

class Event
{
protected:
	double m_dDeadTime;
public:
	Event(double startTime, double duration) :
	  m_dDeadTime(startTime + duration)
	{ }

	bool operator<(Event& other)
	{
		return m_dDeadTime < other.m_dDeadTime;
	}

	virtual void trigger() = 0;

	double getDeadTime() const
	{
		return m_dDeadTime;
	}
};