#pragma once

class GameEvent
{
protected:
	double m_dDeadTime;
public:
	GameEvent(double startTime, double duration) :
	  m_dDeadTime(startTime + duration)
	{ }

	bool operator<(GameEvent & other)
	{
		return m_dDeadTime < other.m_dDeadTime;
	}

	virtual void trigger() = 0;

	double getDeadTime() const
	{
		return m_dDeadTime;
	}
};