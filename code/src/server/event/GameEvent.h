#pragma once

class GameEvent
{
protected:
	double m_dDeadTime;
	bool terminated;
public:
	GameEvent(double startTime, double duration) :
	  m_dDeadTime(startTime + duration),
	  terminated(false)
	{ }

	virtual void trigger() = 0;

	virtual double getDeadTime() const
	{
		return m_dDeadTime;
	}

	void setTerminated()
	{
		terminated = true;
	}
};